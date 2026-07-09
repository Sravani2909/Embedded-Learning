savedcmd_char_driver.mod := printf '%s\n'   char_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > char_driver.mod
