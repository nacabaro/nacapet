#include "lines.h"
#include "memory/memory.h"
#include "defs/defs.h"

#include <SPIFFS.h>

void lines_getSingleLine(const char* fileName) {
    char fullPath[8 + strlen(fileName)];
    snprintf(fullPath, 20, "/lines/%s", fileName);

    File lineFile = SPIFFS.open(fullPath);

    struct Line_t* selectedLine = (struct Line_t*) malloc(sizeof(struct Line_t));
    if (selectedLine == NULL) {
        printf("[LINES] Fallo reserva\n");
    }

    uint8_t buffer[4];

    lineFile.seek(4, SeekCur);

    uint8_t bytesRead = lineFile.read(&selectedLine->id, 1);
    bytesRead += lineFile.readBytes(selectedLine->name, 16);

    Egg_t* selectedEgg = (Egg_t*) malloc(sizeof(Egg_t));
    lines_getSingleEggSprites(lineFile, selectedEgg);

    bytesRead += lineFile.read(buffer, 2);
    selectedLine->hatchTime = (buffer[0] << 8) | buffer[1];
    
    bytesRead += lineFile.read(&selectedLine->charaNumber, 1);

    printf("[DEBUG] SelectedLine numChara=%d hatchTime=%d\n", selectedLine->charaNumber, selectedLine->hatchTime);

    selectedLine->characters = (LineChara_t*) malloc(selectedLine->charaNumber * sizeof(LineChara_t));

    for (int i = 0; i < selectedLine->charaNumber; i++) {
        bytesRead += lineFile.read((uint8_t*) &selectedLine->characters[i], sizeof(LineChara_t));
    }

    currentLine[currentCharacter] = selectedLine;
    currentEgg = selectedEgg;
}

void lines_getSingleEggSprites(File &lineFile, Egg_t* selectedEgg) {
    // Ahora si, vamos a ver cuantos sprites hay que reservar

    // Importante tener el nombre de archivo del huevo en todo momento
    strcpy(selectedEgg->fileName, lineFile.name());

    // Ahora se lee los datos
    lineFile.read(&(selectedEgg->eggSprite.spriteWidth), 1);
    lineFile.read(&(selectedEgg->eggSprite.spriteHeight), 1);
    lineFile.read(&(selectedEgg->eggSprite.spriteNumber), 1);
    
    selectedEgg->eggSprite.spriteData = memory_allocate(
        selectedEgg->eggSprite.spriteNumber, 
        selectedEgg->eggSprite.spriteWidth, 
        selectedEgg->eggSprite.spriteHeight
    );

    uint16_t size = selectedEgg->eggSprite.spriteWidth * selectedEgg->eggSprite.spriteHeight;

    uint8_t highByte;
    uint8_t lowByte;

    for (int spr = 0; spr < selectedEgg->eggSprite.spriteNumber; spr++) {
        for (int i= 0; i < size; i++) {
            lineFile.read(&highByte, 1);
            lineFile.read(&lowByte, 1);

            uint16_t pixel = (highByte << 8) | lowByte;

            selectedEgg->eggSprite.spriteData[spr][i] = pixel;
        }
    } 
}

// Son las 22:35, que estoy haciendo?
// Pues claro

