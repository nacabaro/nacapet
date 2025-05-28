#include "lines.h"
#include "defs/file_chara.h"
#include "defs/defs.h"

#include "SPIFFS.h"

void lines_getLineCareMistakes(const char* fileName) {
    char careMistakesPath[strlen(fileName) + 8];
    snprintf(careMistakesPath, strlen(fileName) + 8, "/care/%s", fileName);

    File careMistakesFile = SPIFFS.open(careMistakesPath);

    uint8_t bytesRead = 0;

    LineCare_t* careMistakesData = (LineCare_t*) malloc(sizeof(LineCare_t));

    careMistakesFile.seek(4, SeekCur);

    bytesRead += careMistakesFile.read(&careMistakesData->lineId, 1);
    bytesRead += careMistakesFile.read(&careMistakesData->numCareMistakesData, 1);

    careMistakesData->careMistakeData = (CareMistakes_t*) malloc(
        sizeof(CareMistakes_t) * careMistakesData->numCareMistakesData
    );

    for (int i = 0; i < careMistakesData->numCareMistakesData; i++) {
        bytesRead += careMistakesFile.read((uint8_t*) &careMistakesData->careMistakeData[i], sizeof(CareMistakes_t));
    }

    currentLineCareInstr[currentCharacter] = careMistakesData; 
}