savedcmd_pcd_ioctl.mod := printf '%s\n'   pcd_ioctl.o | awk '!x[$$0]++ { print("./"$$0) }' > pcd_ioctl.mod
