#include "lines.h"
#include "memory/memory.h"
#include "defs/defs.h"
#include "utils/utils.h"

#include <FS.h>
#include <SPIFFS.h>

const char lineHeader[5] = "NPET";
const uint8_t headerSize = 4;

void lines_getAvailableLines() {
    if (eggSelection != NULL) {
        return;
    }

    fs::File root = SPIFFS.open("/lines");
    fs::File lineFile = root.openNextFile();

    uint8_t allocCount = 0;
    char header[5];

    while (lineFile) {
        printf("[LINES] Opening file %s\n", lineFile.name());

        uint8_t readBytes = lineFile.readBytes(header, headerSize);
        if (strncmp(header, lineHeader, headerSize) == 0 && readBytes == headerSize) {
            allocCount++;
        }

        lineFile = root.openNextFile();
    }

    struct Egg_t* availableLines = (struct Egg_t*) malloc(sizeof(struct Egg_t) * allocCount);

    root.close();

    allocCount = 0;

    root = SPIFFS.open("/lines");
    lineFile = root.openNextFile("r");

    while (lineFile) {
        uint16_t bytesRead = 0;

        bytesRead += lineFile.readBytes(header, headerSize);
        bytesRead += lineFile.read(&availableLines[allocCount].id, 1);
        bytesRead += lineFile.readBytes(availableLines[allocCount].name, 16);

        uint8_t originalWidth;
        uint8_t originalHeight;
        uint8_t spriteCount;

        bytesRead += lineFile.read(&originalWidth, 1);
        bytesRead += lineFile.read(&originalHeight, 1);
        bytesRead += lineFile.read(&spriteCount, 1);

        const uint8_t scaledWidth = originalWidth * SPRITE_SCALE;
        const uint8_t scaledHeight = originalHeight * SPRITE_SCALE;

        availableLines[allocCount].eggSprite.spriteWidth = scaledWidth;
        availableLines[allocCount].eggSprite.spriteHeight = scaledHeight;
        availableLines[allocCount].eggSprite.spriteNumber = 1;
        availableLines[allocCount].eggSprite.spriteData = memory_allocate(1, scaledWidth, scaledHeight);
      
        uint16_t* spriteBuffer =
            (uint16_t*) malloc(
                originalWidth *
                originalHeight *
                sizeof(uint16_t)
            );

        if (!spriteBuffer) {
            printf("[LINES] Failed to allocate sprite buffer\n");

            lineFile.close();
            lineFile = root.openNextFile();

            continue;
        }

        uint8_t highByte;
        uint8_t lowByte;

        for (int i = 0; i < originalWidth * originalHeight; i++) {
            bytesRead += lineFile.read(&highByte, 1);
            bytesRead += lineFile.read(&lowByte, 1);

            spriteBuffer[i] = (highByte << 8) | lowByte;
        }

        utils_upscaleSprite(
            spriteBuffer,
            originalWidth,
            originalHeight,
            availableLines[allocCount].eggSprite.spriteData[0]
        );

        free(spriteBuffer);

        strcpy(availableLines[allocCount].fileName, lineFile.name());

        lineFile.close();

        allocCount++;

        lineFile = root.openNextFile();
    }

    root.close();

    eggNumber = allocCount;
    eggSelection = availableLines;
}