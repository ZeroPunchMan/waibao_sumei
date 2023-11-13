.\toolchain\nrf-command-line-tools\bin\nrfjprog -f nrf52 --eraseall
.\toolchain\nrf-command-line-tools\bin\nrfjprog -f nrf52 --program ..\nrf5_sdk_17.1.0_ddde560\components\softdevice\s112\hex\s112_nrf52_7.2.0_softdevice.hex --sectorerase
.\toolchain\nrf-command-line-tools\bin\nrfjprog -f nrf52 --reset
