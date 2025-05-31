#include "storage.h"
#include "memory/memory.h"
#include "defs/sprite_data.h"

const char* TAG_S = "[STORAGE]";

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

    size_t bytesRead = 0;
    size_t fileSize = file.size();

    uint8_t width, height, spriteNumber;
    
    bytesRead += file.read(&width, 1);
    bytesRead += file.read(&height, 1);
    bytesRead += file.read(&spriteNumber, 1);

    if (spriteData->spriteData != NULL) {
        memory_free(spriteData);
    } 
    
    spriteData->spriteWidth = width;
    spriteData->spriteHeight = height;
    spriteData->spriteNumber = spriteNumber;
    
    spriteData->spriteData = memory_allocate(spriteData->spriteNumber, spriteData->spriteWidth, spriteData->spriteHeight);

    printf(
        "%s Read header: width=%d, height=%d, numSprites=%d\n", 
        TAG_S, spriteData->spriteWidth, spriteData->spriteHeight, spriteData->spriteNumber
    );

    fileSize = (fileSize - 3) / sizeof(uint16_t);

    size_t bufferSize = spriteData->spriteNumber * spriteData->spriteWidth * spriteData->spriteHeight;

    uint8_t highByte;
    uint8_t lowByte;

    for (int sprN = 0; sprN < spriteData->spriteNumber; sprN++) {
        for (int i = 0; i < spriteData->spriteWidth * spriteData->spriteHeight; i++) {
            bytesRead += file.read(&highByte, 1);
            bytesRead += file.read(&lowByte, 1);

            uint16_t pixel = (highByte << 8) | lowByte;

            if (i < bufferSize) {
                (spriteData->spriteData)[sprN][i] = pixel;
            } else {
                printf("%s Buffer overflow, skipping pixel\n", TAG_S);
                break;
            }
        }
    }

    printf("%s Read %zu bytes from file %s\n", TAG_S, bytesRead, path);

    file.close();
}

void storage_initBackground(const char* path, TFT_eSprite& bg) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        printf("%s Failed to open file for reading\n", TAG_S);
        return;
    }

    size_t bytesRead = 0;

    size_t fileSize = file.size();

    uint8_t width;
    uint8_t height;

    bytesRead += file.read(&width, 1);
    bytesRead += file.read(&height, 1);

    printf("%s Read header: width=%d, height=%d\n", TAG_S, width, height);

    fileSize = (fileSize - 2) / sizeof(uint16_t); 

    bg.createSprite(width, height);

    for(int i = 0; i < width * height; i++) {
        uint8_t highByte;
        uint8_t lowByte;

        bytesRead += file.read(&highByte, 1);
        bytesRead += file.read(&lowByte, 1);

        uint16_t pixel = (highByte << 8) | lowByte;

        if (i < fileSize) {
            bg.drawPixel(i % width, i / width, pixel);
        } else {
            printf("%s Buffer overflow, skipping pixel\n", TAG_S);
            break;
        }
    }

    printf("%s Read %zu bytes from file %s\n", TAG_S, bytesRead, path);

    file.close();
}