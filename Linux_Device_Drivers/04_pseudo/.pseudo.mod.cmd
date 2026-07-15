savedcmd_pseudo.mod := printf '%s\n'   pseudo.o | awk '!x[$$0]++ { print("./"$$0) }' > pseudo.mod
