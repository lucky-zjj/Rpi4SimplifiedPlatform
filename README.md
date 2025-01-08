Baremetal programming Platform on Rpi4

this repo is a simplified version based on https://github.com/kth-step/EmbExp-ProgPlatform

armstub:
start4.elf will pass CPU initialization to armstub, start4.elf's cpu initialization will fall into EL2 or EL1 before kernel excution.