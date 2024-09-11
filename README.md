# Home automation system

## ESP32 Development environment setup

### Install VSCode

- Install VSCode Extensions
  - C/C++
  - PlatformIO IDE

### Project setup

- PlatformIO->PIO Home->Open->New Project
- Project name: Your project's name.
- Board: `Espressif ESP32 Dev Module`, this my hardware board.
- Framework: My alternative is `Arduino`.

## AWS LightSail Setup

- Create a new instance
- Choose `Debian 12`
- Choose `CPU 2 vCPU`
- Choose `RAM 2GB`
- Choose `Storage 60GB`
- Instance Network
  - IPv4 Firewall: `All protocols: ALL`

## Install Home Assistant Supervised

Reference: <https://docs.home-assistant.io/installation/supervised-installation/>

### Step 1: Install dependencies

```bash
sudo apt update
sudo apt upgrade
sudo apt install \
apparmor \
bluez \
cifs-utils \
curl \
dbus \
jq \
libglib2.0-bin \
lsb-release \
network-manager \
nfs-common \
systemd-journal-remote \
systemd-resolved \
udisks2 \
wget -y
```

### Step 2: Install Docker-CE

```bash
curl -fsSL get.docker.com | sh
```

### Step 3: Install the OS-Agent

Download the latest Debian package from OS Agent GitHub release page at:

<https://github.com/home-assistant/os-agent/releases/latest>

```bash
wget https://github.com/home-assistant/os-agent/releases/download/1.6.0/os-agent_1.6.0_linux_x86_64.deb 
sudo dpkg -i os-agent_1.6.0_linux_x86_64.deb
```

### Step 4: Install the Home Assistant Supervised Debian Package

```bash
wget -O homeassistant-supervised.deb https://github.com/home-assistant/supervised-installer/releases/latest/download/homeassistant-supervised.deb
sudo apt install ./homeassistant-supervised.deb
```

### Step 5: Fix "Unsupported system - Network Manager issues"

Reference: <https://community.home-assistant.io/t/debian-12-ha-supervisor-installation/640446/19>

> I think I¡¯ve found a solution. The problem is indeed with the cloud images of Debian. They come with netplan and systemd-networkd. This prevents Network Manager from taking over the interface. So the solution is:

```bash
sudo apt remove netplan.io
sudo systemctl disable systemd-networkd
sudo reboot
```

### Step 6: Install EMQX(MQTT Broker)

Reference: <https://docs.emqx.com/en/emqx/latest/deploy/install.html>

#### Home Assistant setting

- Settings -> Add-ons -> Add-on store -> Search EMQX -> Install
- Enter the EMQX server address, port, username, and password
- Turn on `Start on boot` and `Show in sidebar`

### Step 7: Access Home Assistant

- Open a browser and navigate to `http://<lightsail-public-ip>:8123`

#### Add HACS to Home Assistant

Reference: <https://hacs.xyz/docs/use/>

#### Add integration

- Settings -> Devices & Services -> Add Integration
- Search for `MQTT`
- Enter the MQTT server address, port, username, and password
