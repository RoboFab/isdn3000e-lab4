## ISDN3000e-Lab4

This lab requires **Pinocchio** for robot kinematics. Please install it following the commands below.

### Linux / WSL (Ubuntu 20.04 / 22.04 / 24.04)

Run the following command in your Ubuntu/WSL terminal:

```bash
sudo apt install -qqy lsb-release curl

sudo mkdir -p /etc/apt/keyrings
curl http://robotpkg.openrobots.org/packages/debian/robotpkg.asc \
| sudo tee /etc/apt/keyrings/robotpkg.asc

echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/robotpkg.asc] \
http://robotpkg.openrobots.org/packages/debian/pub \
$(lsb_release -cs) robotpkg" \
| sudo tee /etc/apt/sources.list.d/robotpkg.list

sudo apt update
sudo apt install -qqy robotpkg-py3*-pinocchio
```

### MacOS (Homebrew)

Run the following command in your terminal:

```bash
brew tap gepetto/homebrew-gepetto
brew install pinocchio
```