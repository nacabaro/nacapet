#include "lines.h"
#include "memory/memory.h"
#include "defs/defs.h"
#include "utils/utils.h"

#include <FS.h>
#include <SPIFFS.h>

void lines_getSingleLine(const char* fileName) {
    char fullPath[8 + strlen(fileName)];
    snprintf(fullPath, 20, "/lines/%s", fileName);

    fs::File lineFile = SPIFFS.open(fullPath);

    struct Line_t* selectedLine = (struct Line_t*) malloc(sizeof(struct Line_t));
    if (selectedLine == NULL) {
        printf("[LINES] Fallo reserva\n");
    }

    uint8_t buffer[4];

    lineFile.seek(4, fs::SeekCur);

    uint8_t bytesRead = lineFile.read(&selectedLine->id, 1);
    bytesRead += lineFile.readBytes(selectedLine->name, 16);

    Egg_t* selectedEgg = (Egg_t*) malloc(sizeof(Egg_t));
    lines_getSingleEggSprites(lineFile, selectedEgg);

    bytesRead += lineFile.read(buffer, 2);
    selectedLine->hatchTime = (buffer[0] << 8) | buffer[1];
    
    bytesRead += lineFile.read(&selectedLine->charaNumber, 1);

    selectedLine->characters = (LineChara_t*) malloc(selectedLine->charaNumber * sizeof(LineChara_t));

    for (int i = 0; i < selectedLine->charaNumber; i++) {
        bytesRead += lineFile.read((uint8_t*) &selectedLine->characters[i], sizeof(LineChara_t));
    }

    currentLine[currentCharacter] = selectedLine;
    currentEgg = selectedEgg;
}
void lines_getSingleEggSprites(fs::File &lineFile, Egg_t* selectedEgg) {

    // Importante tener el nombre de archivo del huevo en todo momento
    strcpy(selectedEgg->fileName, lineFile.name());

    // Leer dimensiones originales
    uint8_t originalWidth;
    uint8_t originalHeight;

    lineFile.read(&originalWidth, 1);
    lineFile.read(&originalHeight, 1);
    lineFile.read(&(selectedEgg->eggSprite.spriteNumber), 1);

    const uint8_t scaledWidth  = originalWidth  * SPRITE_SCALE;
    const uint8_t scaledHeight = originalHeight * SPRITE_SCALE;

    // Guardar dimensiones escaladas
    selectedEgg->eggSprite.spriteWidth  = scaledWidth;
    selectedEgg->eggSprite.spriteHeight = scaledHeight;

    // Reservar memoria para sprites escalados
    selectedEgg->eggSprite.spriteData = memory_allocate(
        selectedEgg->eggSprite.spriteNumber,
        scaledWidth,
        scaledHeight
    );

    const uint16_t originalSize =
        originalWidth * originalHeight;

    // Buffer temporal en SRAM
    uint16_t* spriteBuffer =
        (uint16_t*) malloc(
            originalSize * sizeof(uint16_t)
        );

    if (!spriteBuffer) {
        printf("[LINES] Failed to allocate sprite buffer\n");
        return;
    }

    uint8_t highByte;
    uint8_t lowByte;

    for (
        int spr = 0;
        spr < selectedEgg->eggSprite.spriteNumber;
        spr++
    ) {

        // Leer sprite original
        for (int i = 0; i < originalSize; i++) {

            lineFile.read(&lowByte, 1);
            lineFile.read(&highByte, 1);

            spriteBuffer[i] =
                (highByte << 8) | lowByte;
        }

        // Escalar sprite
        utils_upscaleSprite(
            spriteBuffer,
            originalWidth,
            originalHeight,
            selectedEgg->eggSprite.spriteData[spr]
        );
    }

    free(spriteBuffer);
}

// Son las 22:35, que estoy haciendo?
// Pues claro

