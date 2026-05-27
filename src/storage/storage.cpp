#include "storage.h"
#include "memory/memory.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "defs/screen_defs.h"
#include "TFT_eSPI.h"

const char* TAG_S = "[STORAGE]";

// All sprites are upscaled by this factor at load time so draw_drawSprite
// can skip the scaling loop entirely at runtime.
#define SPRITE_SCALE 6

void storage_init() {
    if (!SPIFFS.begin(true)) {
        printf("%s Failed to mount file system\n", TAG_S);
    } else {
        printf("%s File system mounted successfully\n", TAG_S);
    }
}


void storage_readFile(const char* path, struct SpriteData* spriteData) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        printf("%s Failed to open file for reading\n", TAG_S);
        return;
    }

    uint8_t width, height, spriteNumber;

    file.read(&width, 1);
    file.read(&height, 1);
    file.read(&spriteNumber, 1);

    if (spriteData->spriteData != NULL) {
        memory_free(spriteData);
    }

    const uint8_t scaledW = width  * SPRITE_SCALE;
    const uint8_t scaledH = height * SPRITE_SCALE;

    // Allocate scaled buffers in PSRAM (ps_malloc falls back to regular heap
    // automatically if PSRAM is not available for a given allocation).
    uint16_t** scaled = (uint16_t**) ps_malloc(spriteNumber * sizeof(uint16_t*));
    if (!scaled) {
        printf("%s PSRAM alloc failed for pointer table\n", TAG_S);
        file.close();
        return;
    }
    for (uint8_t i = 0; i < spriteNumber; i++) {
        scaled[i] = (uint16_t*) ps_malloc(scaledW * scaledH * sizeof(uint16_t));
        if (!scaled[i]) {
            printf("%s PSRAM alloc failed for sprite %d\n", TAG_S, i);
            for (uint8_t j = 0; j < i; j++) free(scaled[j]);
            free(scaled);
            file.close();
            return;
        }
    }

    // Temporary single-row scratch buffer in internal RAM for reading from file
    uint16_t* rowBuf = (uint16_t*) malloc(width * sizeof(uint16_t));
    if (!rowBuf) {
        printf("%s scratch alloc failed\n", TAG_S);
        for (uint8_t i = 0; i < spriteNumber; i++) free(scaled[i]);
        free(scaled);
        file.close();
        return;
    }

    printf(
        "%s Read header: width=%d, height=%d, numSprites=%d -> scaled to %dx%d\n",
        TAG_S, width, height, spriteNumber, scaledW, scaledH
    );

    for (int sprN = 0; sprN < spriteNumber; sprN++) {
        uint16_t* dst = scaled[sprN];

        for (int srcY = 0; srcY < height; srcY++) {
            // --- Read one source row, byte-swapping as we go ---
            for (int srcX = 0; srcX < width; srcX++) {
                uint8_t hi, lo;
                file.read(&hi, 1);
                file.read(&lo, 1);
                // File is big-endian RGB565; TFT_eSPI expects little-endian
                rowBuf[srcX] = (lo << 8) | hi;
            }

            // --- Scale row vertically (repeat SPRITE_SCALE times) ---
            for (int dy = 0; dy < SPRITE_SCALE; dy++) {
                uint16_t* dstRow = dst + (srcY * SPRITE_SCALE + dy) * scaledW;

                // --- Scale each pixel horizontally ---
                for (int srcX = 0; srcX < width; srcX++) {
                    uint16_t color = rowBuf[srcX];
                    uint16_t* dstPixel = dstRow + srcX * SPRITE_SCALE;
                    for (int dx = 0; dx < SPRITE_SCALE; dx++) {
                        dstPixel[dx] = color;
                    }
                }
            }
        }
    }

    free(rowBuf);
    file.close();

    // Store scaled dimensions so the rest of the code sees the final size
    spriteData->spriteWidth  = scaledW;
    spriteData->spriteHeight = scaledH;
    spriteData->spriteNumber = spriteNumber;
    spriteData->spriteData   = scaled;

    printf("%s Loaded & upscaled %s (%d sprites, each %dx%d px)\n",
           TAG_S, path, spriteNumber, scaledW, scaledH);
}

void storage_initBackground(const char* path, TFT_eSprite& bg) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        printf("%s Failed to open file for reading\n", TAG_S);
        return;
    }

    uint8_t width, height;
    file.read(&width, 1);
    file.read(&height, 1);

    size_t pixelCount = width * height;

    printf("%s width=%d height=%d pixels=%zu\n", TAG_S, width, height, pixelCount);

    bg.createSprite(width, height);

    uint16_t* bgBuf = (uint16_t*) bg.getPointer();

    for (size_t i = 0; i < pixelCount; i++) {
        uint8_t hi, lo;
        file.read(&lo, 1);
        file.read(&hi, 1);
        // Store directly into sprite buffer – no byte swap needed here since
        // background pixels are not going through the draw_drawSprite path.
        bgBuf[i] = (hi << 8) | lo;
    }

    file.close();
}

void storage_saveState() {
    File file;

    file = SPIFFS.open("/save.bin", "w");

    if (!file) {
        printf("%s Cannot save: cannot open save file.\n", TAG_S);
        return;
    }

    file.write(
        (uint8_t*) charaData, 
        sizeof(struct CharacterData) * CHARA_COUNT_IN_DEVICE
    );

    printf("%s Saved current character data.\n", TAG_S);

    for (int i = 0; i < CHARA_COUNT_IN_DEVICE; i++) {
        printf("%s Saving line data %i\n", TAG_S, i);

        uint8_t lineUsed = (currentLine[i] != NULL);
        file.write(lineUsed);

        if (lineUsed) {
            file.write(currentLine[i]->id);
            file.write((uint8_t*) &(currentLine[i]->hatchTime), sizeof(uint16_t));
            file.write((uint8_t*) currentLine[i]->name, 16);
            file.write(currentLine[i]->charaNumber);
            
            printf("%s Done saving static line %i\n", TAG_S, i);
    
            uint8_t hasCharacters = (currentLine[i]->characters != NULL);
            file.write(hasCharacters);
    
            printf("%s hasCharacters=%i, characters=%i, charaNumber=%i\n", 
                    TAG_S, hasCharacters, currentLine[i]->characters, currentLine[i]->charaNumber);
            
            if (hasCharacters) {
                file.write(
                    (uint8_t*) currentLine[i]->characters, 
                    sizeof(LineChara_t) * currentLine[i]->charaNumber
                );
            }
    
            printf("%s Done saving line data for %i\n", TAG_S, i);
        } 
    }

    for (int i = 0; i < CHARA_COUNT_IN_DEVICE; i++) {
        printf("%s Saving care data %i\n", TAG_S, i);
        
        uint8_t careDataUsed = currentLineCareInstr[i] != NULL;
        file.write(careDataUsed);

        if (careDataUsed) {
            file.write(currentLineCareInstr[i]->lineId);
            file.write(currentLineCareInstr[i]->numCareMistakesData);
    
            printf("%s Done saving static care data %i\n", TAG_S, i);
    
            uint8_t hasCareData = (currentLineCareInstr[i]->careMistakeData != NULL); 
            file.write(hasCareData);
    
            printf("%s hasCharacters=%i, characters=%i, charaNumber=%i\n", 
                    TAG_S, hasCareData, currentLineCareInstr[i]->careMistakeData, currentLineCareInstr[i]->numCareMistakesData);
    
            if (hasCareData) {
                file.write(
                    (uint8_t*) currentLineCareInstr[i]->careMistakeData, 
                    sizeof(CareMistakes_t) * currentLineCareInstr[i]->numCareMistakesData
                );
            }
    
            printf("%s Done saving care data for %i\n", TAG_S, i);
        }

    }

    file.write(currentCharacter);

    file.close();

    printf("%s Save completed!\n", TAG_S);
}

void storage_loadState() {
    File file;

    if (!SPIFFS.exists("/save.bin")) {
        printf("%s Cannot load: File does not exist.\n", TAG_S);
        return;
    }

    file = SPIFFS.open("/save.bin", "r");

    if (!file) {
        printf("%s Cannot save: cannot open save file.\n", TAG_S);
        return;
    }

    file.read((uint8_t*) charaData, sizeof(struct CharacterData) * CHARA_COUNT_IN_DEVICE);

    for (int i = 0; i < CHARA_COUNT_IN_DEVICE; i++) {
        uint8_t currentLineUsed = file.read();

        if (currentLineUsed) {
            currentLine[i] = (Line_t*) calloc(1, sizeof(Line_t));

            currentLine[i]->id = file.read();
            file.read((uint8_t*) &(currentLine[i]->hatchTime), sizeof(uint16_t));
            file.read((uint8_t*) currentLine[i]->name, 16);
            currentLine[i]->charaNumber = file.read();
            
            uint8_t hasCharacters = file.read();

            if (hasCharacters) {
                if (currentLine[i]->characters != NULL) {
                    free(currentLine[i]->characters);
                }

                currentLine[i]->characters = (LineChara_t*) malloc(sizeof(LineChara_t) * currentLine[i]->charaNumber);
                file.read(
                    (uint8_t*) currentLine[i]->characters, 
                    sizeof(LineChara_t) * currentLine[i]->charaNumber
                );
            } else {
                currentLine[i]->characters = NULL;
            }
        }

    }

    for (int i = 0; i < CHARA_COUNT_IN_DEVICE; i++) {
        uint8_t currentLineUsed = file.read();

        if (currentLineUsed) {
            currentLineCareInstr[i] = (LineCare_t*) calloc(1, sizeof(LineCare_t));
            
            currentLineCareInstr[i]->lineId = file.read();
            currentLineCareInstr[i]->numCareMistakesData = file.read();
        
            uint8_t hasCareData = file.read();
            if (hasCareData) {
                if (currentLineCareInstr[i]->careMistakeData != NULL) {
                    free(currentLineCareInstr[i]->careMistakeData);
                }
                
                currentLineCareInstr[i]->careMistakeData = (CareMistakes_t*) malloc(sizeof(CareMistakes_t) * currentLineCareInstr[i]->numCareMistakesData);
                file.read(
                    (uint8_t*) currentLineCareInstr[i]->careMistakeData, 
                    sizeof(CareMistakes_t) * currentLineCareInstr[i]->numCareMistakesData
                );
            } else {
                currentLineCareInstr[i]->careMistakeData = NULL;
            }
        }
    }

    currentCharacter = file.read();

    file.close();

    char spriteFileName[30];
    snprintf(spriteFileName, 30, "/chara/%02x.bin", charaData[currentCharacter].idChara);

    storage_readFile(spriteFileName, &mainCharacterSprites);

    printf("%s Load completed!\n", TAG_S);
}