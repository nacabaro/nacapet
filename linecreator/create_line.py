import re
import sys
import logging

def process_npet_file(origFp, eggFp, outputName):
    file_header = origFp.readline()
    
    matches = re.findall(r"\[\s*([0-9a-fA-F]+)\s*\|\s*([^|]+?)\s*\|\s*([0-9a-fA-F]+)\s*\|\s*([0-9a-fA-F]+)\s*\]", file_header)
    
    if len(matches[0]) != 4:
        logging.error("Cabecera invalida")
        return
    
    id, nombre, numChara, hatchTime = matches[0]
    id = int(id, 16)
    numChara = int(numChara, 16)
    hatchTime = int(hatchTime, 16)
    
    logging.info(f"Encontrado cabecera id={id}, nombre={nombre}, numchara={numChara}")

    destFp = open(outputName, "wb")
    
    destFp.write(b"NPET")
    
    destFp.write(id.to_bytes(1, "big"))
    destFp.write(nombre.encode("utf8").ljust(16, b"\0"))
    
    eggSpriteData = eggFp.read()
    
    destFp.write(eggSpriteData)
    
    destFp.write(hatchTime.to_bytes(2, "big"))
    destFp.write(numChara.to_bytes(1, "big"))

    for i in range(numChara):
        line = origFp.readline()
        matches = re.findall(
            r"\[\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([^|]+?)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*"
            r"\]$", line 
        )
        if len(matches[0]) != 13:
            logging.error("Estructura incorrecta, necesita 13 parametros.")
            return
        
        charaId = int(matches[0][0], 16)
        charaName = matches[0][1].encode("utf-8").ljust(16, b"\0")
        charaHp = int(matches[0][2], 16)
        charaBp = int(matches[0][3], 16)
        charaAp = int(matches[0][4], 16)
        charaStage = int(matches[0][5], 16)
        charaAttribute = int(matches[0][6], 16)
        charaAttackSprite = int(matches[0][7], 16)
        charaSleepTime = int(matches[0][8], 16)
        charaWakeupTime = int(matches[0][9], 16)
        charaEvolutionTime = int(matches[0][10], 16)
        charaReductionTime = int(matches[0][11], 16)
        charaMinWeight = int(matches[0][12], 16)
        
        destFp.write(charaId.to_bytes(1, "big"))
        destFp.write(charaName)
        destFp.write(charaHp.to_bytes(1, "big"))
        destFp.write(charaBp.to_bytes(1, "big"))
        destFp.write(charaAp.to_bytes(1, "big"))
        destFp.write(charaStage.to_bytes(1, "big"))
        destFp.write(charaAttribute.to_bytes(1, "big"))
        destFp.write(charaAttackSprite.to_bytes(1, "big"))
        destFp.write(charaSleepTime.to_bytes(4, "big"))
        destFp.write(charaWakeupTime.to_bytes(4, "big"))
        destFp.write(charaEvolutionTime.to_bytes(4, "big"))
        destFp.write(charaReductionTime.to_bytes(2, "big"))
        destFp.write(charaMinWeight.to_bytes(1, "big"))
        
        logging.info(f"Añadiendo nueva entrada con nombre {matches[0][1]}")
    
    destFp.close()
    
    logging.info("Creado archivo de manera correcta")


def process_nevo_file(origFp, outputName):
    file_header = origFp.readline()
    
    matches = re.findall(
        r"\[\s*"
        r"([0-9a-fA-F]+)\s*\|\s*"          # Group 1: First single hex (e.g., 00)
        r"([0-9a-fA-F]+)\s*"
        r"\]$", file_header
    )
    
    if (len(matches[0]) != 2):
        logging.error("Cabecera invalida")
        return

    id, length = [ int(i, 16) for i in matches[0] ]

    logging.info(f"Encontrado cabecera id={id} numEvos={length}")
    
    destFp = open(outputName, "wb")
    
    destFp.write(b"NEVO")
        
    destFp.write(id.to_bytes(1, "big"))
    destFp.write(length.to_bytes(1, "big"))
    
    for i in range(length):
        line = origFp.readline()
        
        if (line.startswith("#")):
            continue
        
        pattern = re.findall(
            r"\[\s*"
            r"([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\|\s*"
            r"([0-9a-fA-F]+)\s*"
            r"\]$", line
        )
                
        charaId, charaNextId = [ int(i, 16) for i in pattern[0][0:2] ]
        careMistakeMin, careMistakeMax = [ int(i, 16) for i in pattern[0][2:4] ]
        sleepDistMin, sleepDistMax = [ int(i, 16) for i in pattern[0][4:6] ]
        overfeedMin, overfeedMax = [ int(i, 16) for i in pattern[0][6:8] ]
        trainMin, trainMax = [ int(i, 16) for i in pattern[0][8:10] ]
        trainMin, trainMax = [ int(i, 16) for i in pattern[0][8:10] ]
        totalWon, totalBattled = [ int(i, 16) for i in pattern[0][10:12] ]
        
        evoData = bytearray([
            charaId, charaNextId,
            careMistakeMin, careMistakeMax,
            sleepDistMin, sleepDistMax,
            overfeedMin, overfeedMax,
            trainMin, trainMax,
            totalBattled, totalWon
        ])
        
        destFp.write(evoData)
        
    destFp.close()
    
    logging.info("Creado archivo de manera correcta")
        
    
if __name__ == "__main__":
    if len(sys.argv) < 3:
        logging.info(f"Uso: {sys.argv[0]} <nombre archivo texto plano> <archivo salida>")
        sys.exit(-1)
    
    origFile, newFile = sys.argv[1:]
    logging.basicConfig(level=0, format="%(levelname)s - %(message)s")
        
    origFp = open(origFile, "r")
    
    header = origFp.readline()
    if "NPET" in header:
        logging.info("Cabecera NPET encontrada!")
        origFileName = "".join(origFile.split(".")[0:-1]) + ".egg"
        eggFp = open(origFileName, "rb")
        process_npet_file(origFp, eggFp, newFile)
        eggFp.close()
        
    elif "NEVO" in header:
        logging.info("Cabecera NEVO encontrada!")
        process_nevo_file(origFp, newFile)  
    
    else:
        logging.error("No se ha encontrado una cabecera válida.")

        
    sys.exit(0)