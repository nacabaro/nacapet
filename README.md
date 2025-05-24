# NacaPet

Small implementation of a VPet (Virtual Pet) using an ESP32, an MPU6050, a speaker and an ST7789 display.

## Beware

This code has been written in Spanglish, not the best practice, I know. I just wanted to try out something and never planned to release it, so yeah. Sorry!

## Another beware

Graphical assets are missing, you will have to create your own. You can find more information as to how these assets are laid out in the file defs.h.

## Hardware

For this you will need an standard ESP32, an MPU6050, since this also counts steps, a speaker, a 240x240 TFT and 4 buttons. There is one display in AliExpress which also contains the buttons too in the display unit. You can see more information on how to connect all the devices in the `defs.h` in exception of the display, which has to be configured manually through TFT_eSPI.

## Assets

Assets are composed by 16x16 sprites. can be laid out in a PNG file all together. Once the assets are created in the PNG file, they have to be converted into an RGB565 following the data strucutre this thing wants. For that, check out `scripts/png_to_rgb565.py`. Aside from that you have both the menu assets and the ui assets (menu.bin and ui.bin). menu.bin is composed by 16x16 sprites, ui.bin is composed by 8x8 sprites. Then the background layer (bg.bin) there is a script for that one inside the scripts folder. Background ideally has to be 90x90, otherwise I will be running out of memory real quick. Finally the main character `sprite.bin`, which follows the same structure as the menu sprites, and can be generated through the same script.
