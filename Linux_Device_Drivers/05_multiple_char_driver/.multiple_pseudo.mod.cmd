savedcmd_multiple_pseudo.mod := printf '%s\n'   multiple_pseudo.o | awk '!x[$$0]++ { print("./"$$0) }' > multiple_pseudo.mod
