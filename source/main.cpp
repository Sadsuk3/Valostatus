#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib> // Para ejecutar comandos del sistema

bool musicaPausada = false;
bool idiomaIngles = false; // Variable para seguir el idioma actual

void toggleMusica(sf::Music& music) {
    if (musicaPausada) {
        music.play();
        musicaPausada = false;
    }
    else {
        music.pause();
        musicaPausada = true;
    }
}

void bloquearModoDesconectado() {
    // Comando para bloquear el puerto 5223 en modo desconectado
    std::string comando = "netsh advfirewall firewall add rule name=\"lolchat\" dir=out remoteport=5223 protocol=TCP action=block";
    system(comando.c_str());
}

void desbloquearModoConectado() {
    // Comando para desbloquear el puerto 5223 en modo conectado
    std::string comando = "netsh advfirewall firewall delete rule name=\"lolchat\"";
    system(comando.c_str());
}

int main() {
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(800, 450), "Valorant Status");

    // Crear un objeto para cargar la música
    sf::Music music;
    if (!music.openFromFile("sound.wav")) {
        std::cout << "Error al cargar el archivo de música." << std::endl;
        return 1;
    }
    //forzar español
    idiomaIngles = false;
    // Reproducir la música en bucle
    music.setLoop(true);
    music.play();

    // Cargar una textura para el fondo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fondo.png")) {
        std::cout << "Error al cargar el archivo de fondo." << std::endl;
        return 1;
    }
    sf::Sprite background(backgroundTexture);

    // Fuente y texto para las instrucciones
    sf::Font font;
    if (!font.loadFromFile("8bit.ttf")) {
        std::cout << "Error al cargar la fuente." << std::endl;
        return 1;
    }
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::Black);

    // Texto para mostrar el idioma actual
    sf::Text languageText;
    languageText.setFont(font);
    languageText.setCharacterSize(12);
    languageText.setFillColor(sf::Color::Black);
    languageText.setString("Idioma: Español");
    languageText.setPosition(20.f, 360.f);

    // Texturas y sprites para los botones de cambio de idioma
    sf::Texture spanishButtonTexture;
    sf::Texture englishButtonTexture;
    if (!spanishButtonTexture.loadFromFile("spanish_button.png") || !englishButtonTexture.loadFromFile("english_button.png")) {
        std::cout << "Error al cargar los archivos de textura de los botones." << std::endl;
        return 1;
    }
    sf::Sprite spanishButtonSprite(spanishButtonTexture);
    sf::Sprite englishButtonSprite(englishButtonTexture);
    spanishButtonSprite.setPosition(20.f, 380.f);
    englishButtonSprite.setPosition(80.f, 380.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
                    bloquearModoDesconectado();
                    std::cout << "Modo desconectado activado. El puerto 5223 esta bloqueado. OFFLINE MODE (port 5223 blocked)" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2) {
                    desbloquearModoConectado();
                    std::cout << "Modo conectado activado. El puerto 5223 esta desbloqueado. ONLINE MODE (port 5223 unblocked)" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0) {
                    toggleMusica(music);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Verificar si se hizo clic en el botón de cambio de idioma
                sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                if (spanishButtonSprite.getGlobalBounds().contains(mousePosition)) {
                    // Cambiar al idioma español
                    idiomaIngles = false;
                    languageText.setString("Idioma: Español");
                    instructions.setString("Presiona 1 para Modo Desconectado\nPresiona 2 para Modo Conectado\nPresiona 0 para pausar/reanudar la música");
                }
                else if (englishButtonSprite.getGlobalBounds().contains(mousePosition)) {
                    // Cambiar al idioma inglés
                    languageText.setString("Idioma: English");
                    idiomaIngles = true;
                    instructions.setString("Press 1 for Disconnected Mode\nPress 2 for Connected Mode\nPress 0 to pause/resume music");
                }
            }
        }
        // Dibujar el fondo, los botones y las instrucciones en la ventana
        window.clear();
        window.draw(background);
        window.draw(spanishButtonSprite);
        window.draw(englishButtonSprite);
        window.draw(instructions);
        window.draw(languageText);
        window.display();
    }

    return 0;
}