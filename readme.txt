Vyvojove poznamky k Ras Racku
SPO 25.02.2026
Medicom a.s. Praha

----------------------------------------------------------------------------------------------------------------------------------------------------------
Pro budouci instalace se bude kopirovat image z SD karty - toto nastaveni uz nebude potreba
----------------------------------------------------------------------------------------------------------------------------------------------------------

25.02.2026 Prvni nastaveni

Nastaveni Raspberry Pi 4b

	- Instalace nejnovejsi verze RPi OS 64 bit
		- Pouzit Raspberry Imager - I:/electronics/Raspberry Pi/
		- Povolit SSH
		- Device name:	raspberrypi4b
		- Username:		pi
		- Password:		Ras.00538078

	- Pomoci gitu ziskat nasledujici knihovny
		- WiringPi:
			
			# fetch the source
			sudo apt install git
			git clone https://github.com/WiringPi/WiringPi.git
			cd WiringPi

			# build the package
			./build debian
			mv debian-template/wiringpi-3.x.deb 
	
			# install it
			sudo apt install ./wiringpi-3.x.deb

			Otestovat pomoci prikazu gpio readall

		- RPi IO Board C++ Library:
			git clone https://github.com/abelectronicsuk/ABElectronics_CPP_Libraries.git
	
Nastaveni AutoRun pro Ras Rack program

	- Nastaveni je provedeno pomoci service
	
	- Nastavit moznost pousteni bez zadani directory
			cd /home/pi/progname
			chmod +x progname

	- Zalozit service file
			sudo nano /etc/systemd/system/name.service

	- Vyplnit service file pomoci nano
		- aktualizovano pro aplikaci s GUI

			[Unit]
			Description=GPIO Blink Program
			After=graphical.target

			[Service]
			ExecStart=/home/pi/Blink
			WorkingDirectory=/home/pi
			Restart=always
			Environment=DISPLAY=:0
			Environment=XAUTHORITY=/home/pi/.Xauthority

			[Install]
			WantedBy=default.target

			Ulozit pomoci Ctrl + X -> Y -> Enter

	- Povolit sluzbu
			sudo systemctl daemon-reload
			sudo systemctl enable name.service
			sudo systemctl start name.service

	- Check status
			sudo systemctl status name.service

	- Test status
			sudo systemctl start name.service

	- Rebootovat, prog by uz mel nabehnout
			sudo reboot


SSH Pripojeni z Win PC
	- Pouzivat MobaXTerm (superset PuTTy)
	- New session -> SSH
	- Device Name:	raspberrypi4b
	- Username:		pi
	- Password:		Ras.00538078					- Při SSH pripojeni se pri zadavani hesla do radku nic nezobrazuje


Pripojeni pres Visual Studio
	- Stahnout novy projekt RasRack z TFS
	- Pri spusteni programu zadat prihlasovaci udaje
	- Device Name:	raspberrypi4b
	- Username:		pi
	- Password:		Ras.00538078
	
Zprovozneni GUI pomoci knihovny	#include <gtkmm.h>
	- instalace pomoci sudo apt install libgtkmm-3.0-dev     (moznoi i verzi 4.0 )
	- Dokumentace viz https://gtkmm.gnome.org/en/index.html   https://gtkmm.gnome.org/en/documentation.html

----------------------------------------------------------------------------------------------------------------------------------------------------------

Nastaveni projektu pro remote debug na RPi pres Win11 PC

	- Pouziti knihoven pro tvorbu GUI GTKmm zpusobuje problemy
	- Projekt nelze compilovat na x86/64 architekture 
		- Knihovny GTKmm obsahuji jen a pouze ARM/ARM64 variantu
	- Je potreba rucne vytvorit isntrukce pro plne podporovany cross-compiler obsahujici sysroot cesty ke vsem knihovnam na RPi
		- Neni to typicky ani doporucovany postup
		- Pretrvava problem toho, ze projekt nelze compilovat bez RPi
		- Samotna kompilace probehne na Win11 PC ve virtualni instanci linuxu (WSL), ale pres kompilator je pres SSH odkazovan na soubory na samotnem RPi
		- Je neustale nutne mit spusten WSL a pripojeni k RPi
			- VS Code bezi ve WSL


Varianta 1: Full Remote Debug na RPi pomoci VSCode na Win11 PC
	- Podporuje remote debug s nahledem do kodu za behu pomoci VSCode
	- Cely projekt na GitHubu
	- Vzdalena uprava na RPi pomoci VSCode
	- Bezi pres SSH

	- Pros:
		- Jednoduche
		- Rychle
		- Snadne pridavat do projektu dalsi knihovny
		- Plne podporovane ze strany VSCode i RPi
		- Standardni postup pro obdobne projekty

	- Cons:
		- Projekt se kompiluje na RPi - nelze sestavit lokalne bez pristupu k RPi

Varinata 2: Plny cross-compiler podporujici ARM64 a x86/64
	- Projekt ulozeny v PC
	- Kompilace na samotnem PC pomoci virtualni instance Linuxu - WSL
	
	- Pros:
		- Projekt lze sestavit na samotnem PC - i presto musi byt pripojeno RPi - potrebuje pristup do sysrootu

	- Cons:
		- Velmi slozite nastaveni
		- Nutnost mit WSL - virtualni instance linuxu
		- Musi se rucne sepsat instrukce pro cross compilator
			- Potreba rucne najit a zahrnout veskere knihovny pouzite v projektu - GTKmm jich obsahuje desitky
			- Rucne je nalinkovat uze sysrootu
		- Stale neni mozna kompilace bez RPi

Varianta 3: Vypustit GUI a z projektu odebrat GTKmm
	- Nejvetsi obtize pri kompilaci zpusobuje GTKmm
		- Jedna se o wrapper nekolika knihoven do C
		- Nejsnazsi a defacto jediny hromadne pouzivany zpusov tvorby GUI na RPi
			- Jedina dalsi moznost je QT, ale neni jasne, zda je podporovano - v kazdem pripade zpusobi stejne problemy

	- Pri vynechani GTKmm a pouzizi samotneho WiringPi neni problem pouzit kompilovani na Win11 PC
		- Je mozne ze dalsi knihovny opet zpusobi problemy - napr IO Board knihovna (opet shluk nekolika dilcich knihoven)







