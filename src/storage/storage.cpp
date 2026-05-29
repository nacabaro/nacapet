#include "storage.h"
#include "memory/memory.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "defs/screen_defs.h"
#include "TFT_eSPI.h"

const char* TAG_S = "[STORAGE]";

void storage_init() {
    if (!SPIFFS.begin(true)) {
        printf("%s Failed to mount file system\n", TAG_S);
    } else {
        printf("%s File system mounted successfully\n", TAG_S);
    }
}


void storage_initBackground(const int id, TFT_eSprite& bg) {
    char path[15];

    snprintf(path, 15, "/bg/%i.bin", id);

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

bool storage_deleteState() {
    const char* savePath = "/save.bin";

    if (!SPIFFS.exists(savePath)) {
        printf("%s Save file already deleted.\n", TAG_S);
        return true;
    }

    if (!SPIFFS.remove(savePath)) {
        printf("%s Failed to delete save file.\n", TAG_S);
        return false;
    }

    printf("%s Save file deleted.\n", TAG_S);
    return true;
}
