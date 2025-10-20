#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath> // Necesario para sqrt

using namespace std;
using namespace sf;

enum class GameState{
    MENU,
    PLAYING,
    GAME_OVER,
    EXIT
};

// Función para aplicar aceleración progresiva por rebotes (MODIFICADA)
void aplicarAceleracionPorRebote(Vector2f& ballSpeed, float velocidadBase, float incremento, int rebotes, float maxVelocidad) {
    float velocidadActual = velocidadBase + (incremento * rebotes);
    
    // Limitar la velocidad máxima
    if (velocidadActual > maxVelocidad) {
        velocidadActual = maxVelocidad;
    }
    
    // Aplicar la velocidad actual manteniendo la dirección
    float magnitudActual = sqrt(ballSpeed.x * ballSpeed.x + ballSpeed.y * ballSpeed.y);
    ballSpeed.x = (ballSpeed.x / magnitudActual) * velocidadActual;
    ballSpeed.y = (ballSpeed.y / magnitudActual) * velocidadActual;
}

GameState showMenu(RenderWindow& window, Font& font) {
    Text titleText(font, "PONG GAME", 60);
    Text startText(font, "Presiona ESPACIO para jugar", 24);
    Text exitText(font, "Presiona ESC para salir", 20);
    
    // Configurar textos
    titleText.setFillColor(Color::White);
    startText.setFillColor(Color::Green);
    exitText.setFillColor(Color::Cyan);
    
    // Posiciones fijas - sin usar bounds
    titleText.setPosition(Vector2f(150.0f, 150.0f));
    startText.setPosition(Vector2f(125.0f, 300.0f));
    exitText.setPosition(Vector2f(215.0f, 400.0f));
    
    Clock blinkClock;
    bool showStartText = true;
    
    while (window.isOpen()) {
        // Manejo de eventos en el menú
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                return GameState::EXIT;
            }
            
            if (event->is<Event::KeyPressed>()) {
                auto keyEvent = event->getIf<Event::KeyPressed>();
                if (keyEvent && keyEvent->code == Keyboard::Key::Space) {
                    return GameState::PLAYING;
                }
                if (keyEvent && keyEvent->code == Keyboard::Key::Escape) {
                    return GameState::EXIT;
                }
            }
        }
        
        // Efecto de parpadeo para el texto de inicio
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f) {
            showStartText = !showStartText;
            blinkClock.restart();
        }
        
        if (showStartText) {
            startText.setFillColor(Color::Green);
        } else {
            startText.setFillColor(Color::Transparent);
        }
        
        // Dibujado del menú
        window.clear(Color::Black);
        window.draw(titleText);
        window.draw(startText);
        window.draw(exitText);
        window.display();
    }
    
    return GameState::EXIT;
}

GameState showGameOver(RenderWindow& window, Font& font, int ganador) {
    Text titleText(font, "JUEGO TERMINADO", 40);
    Text ganadorText(font, "GANADOR: JUGADOR " + to_string(ganador), 40);
    Text menuText(font, "Presiona ESPACIO para volver al menu", 20);
    Text exitText(font, "Presiona ESC para salir", 20);
    
    // Configurar textos
    titleText.setFillColor(Color::Yellow);
    ganadorText.setFillColor(Color::Green);
    menuText.setFillColor(Color::Cyan);
    exitText.setFillColor(Color::White);
    
    // Posiciones
    titleText.setPosition(Vector2f(120.0f, 150.0f));
    ganadorText.setPosition(Vector2f(90.0f, 250.0f));
    menuText.setPosition(Vector2f(90.0f, 350.0f));
    exitText.setPosition(Vector2f(215.0f, 430.0f));
    
    Clock blinkClock;
    bool showMenuText = true;
    
    while (window.isOpen()) {
        // Manejo de eventos en game over
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                return GameState::EXIT;
            }
            
            if (event->is<Event::KeyPressed>()) {
                auto keyEvent = event->getIf<Event::KeyPressed>();
                if (keyEvent && keyEvent->code == Keyboard::Key::Space) {
                    return GameState::MENU;
                }
                if (keyEvent && keyEvent->code == Keyboard::Key::Escape) {
                    return GameState::EXIT;
                }
            }
        }
        
        // Efecto de parpadeo para el texto del menú
        if (blinkClock.getElapsedTime().asSeconds() > 0.5f) {
            showMenuText = !showMenuText;
            blinkClock.restart();
        }
        
        if (showMenuText) {
            menuText.setFillColor(Color::Cyan);
        } else {
            menuText.setFillColor(Color::Transparent);
        }
        
        // Dibujado de game over
        window.clear(Color::Black);
        window.draw(titleText);
        window.draw(ganadorText);
        window.draw(menuText);
        window.draw(exitText);
        window.display();
    }
    
    return GameState::EXIT;
}

int main(){
    //Crear la ventana del juego
    RenderWindow ventana(VideoMode({800, 600}), "Pong - SFML 3.0.2");
    ventana.setFramerateLimit(60);
    
    //Estado del juego
    GameState currentState = GameState::MENU;

    //Variables de Puntuacion
    int puntosPlayer1 = 0;
    int puntosPlayer2 = 0;
    const int PUNTOS_VICTORIA = 1;

    // Variable para almacenar el ganador
    int ganadorGlobal = 0;

    //Sistema de texto
    Font fuente;

    if(!fuente.openFromFile("assets/Pixeled.ttf")){
        //si no encuentra la fuente
        cout<<"Error al cargar la fuente"<<endl;
        return -1;
    }  

    Text textoPuntos1(fuente,"0",40);
    Text textoPuntos2(fuente,"0",40);

    //Texto player1
    textoPuntos1.setFillColor(Color::White);
    textoPuntos1.setPosition(Vector2f(300.0f, 20.0f));
    //Texto player2
    textoPuntos2.setFillColor(Color::White);
    textoPuntos2.setPosition(Vector2f(480.0f, 20.0f));

    //Paleta 1 (izquierda)
    RectangleShape player1;
    player1.setSize(Vector2f(15.0f, 100.0f));
    player1.setPosition(Vector2f(50.0f, 250.0f));
    player1.setFillColor(Color::White);

    //Paleta 2 (derecha)
    RectangleShape player2;
    player2.setSize(Vector2f(15.0f, 100.0f));
    player2.setPosition(Vector2f(735.0f, 250.0f));
    player2.setFillColor(Color::White);

    //Pelota
    CircleShape ball(10.0f);
    ball.setPosition(Vector2f(390.0f, 290.0f));
    ball.setFillColor(Color::White);

    //Linea Central
    RectangleShape segment;
    segment.setSize(Vector2f(5.0f, 20.0f));
    segment.setFillColor(Color::White);

    //Variable de velocidad
    float paletaSpeed = 8.0f;
    Vector2f ballSpeed(5.0f, 3.0f);

    float velocidadBase = 5.0f;     //Velocidad inicial
    float incremento = 0.3f;        //Aumento por cada rebote
    float maxVelocidad = 15.0f;     //Velocidad Maxima
    int rebotes = 0;                //contador de rebotes

    //Bucle principal del juego
    while(ventana.isOpen()){
        switch(currentState){
            case GameState::MENU:
                currentState = showMenu(ventana, fuente);
                break;
            
            case GameState::PLAYING:
                {
                    // Manejo de eventos en el juego
                    while (auto event = ventana.pollEvent()) {
                        if (event->is<Event::Closed>()) {
                            ventana.close();
                        }
                        
                        // Salir al menú con ESC
                        if (event->is<Event::KeyPressed>()) {
                            auto keyEvent = event->getIf<Event::KeyPressed>();
                            if (keyEvent && keyEvent->code == Keyboard::Key::Escape) {
                                currentState = GameState::MENU;
                                // Resetear puntuación al volver al menú
                                puntosPlayer1 = 0;
                                puntosPlayer2 = 0;
                                textoPuntos1.setString("0");
                                textoPuntos2.setString("0");
                                rebotes=0; //Resetear rebotes

                                player1.setPosition(Vector2f(50.0f, 250.0f));
                                player2.setPosition(Vector2f(735.0f, 250.0f));
                                ball.setPosition(Vector2f(390.0f, 290.0f));
                                ballSpeed = Vector2f(5.0f, 3.0f); // Resetear velocidad también
                            }
                        }
                    }

                    //Controles Player1 (W-S)
                    if(Keyboard::isKeyPressed(Keyboard::Key::W)){
                        player1.move(Vector2f(0.0f, -paletaSpeed));

                        if(player1.getPosition().y < 0.0f){
                            player1.setPosition(Vector2f(50.0f, 0.0f));
                        }
                    }
                    if(Keyboard::isKeyPressed(Keyboard::Key::S)){
                        player1.move(Vector2f(0.0f, paletaSpeed));

                        if(player1.getPosition().y > 500.0f){
                            player1.setPosition(Vector2f(50.0f, 500.0f));
                        }
                    }

                    //Controles Player2 (Flechas)
                    if(Keyboard::isKeyPressed(Keyboard::Key::Up)){
                        player2.move(Vector2f(0.0f, -paletaSpeed));

                        if(player2.getPosition().y < 0.0f){
                            player2.setPosition(Vector2f(735.0f, 0.0f));
                        }
                    }
                    if(Keyboard::isKeyPressed(Keyboard::Key::Down)){
                        player2.move(Vector2f(0.0f, paletaSpeed));

                        if(player2.getPosition().y > 500.0f){
                            player2.setPosition(Vector2f(735.0f, 500.0f));
                        }
                    }

                    //Limites de la Pelota
                    ball.move(ballSpeed);
                    float ballTop = ball.getPosition().y;
                    float ballBottom = ballTop + 20.0f;

                    //Rebote en el techo (y<0)
                    if (ballTop < 0.0f){
                        ballSpeed.y = -ballSpeed.y;
                        ball.setPosition(Vector2f(ball.getPosition().x, 0.0f));
                    }
                    //Rebote en el suelo   
                    if (ballBottom > 600.0f){
                        ballSpeed.y = -ballSpeed.y;
                        ball.setPosition(Vector2f(ball.getPosition().x, 580.0f));
                    }
                    //Detectar anotacion
                    float ballX = ball.getPosition().x;
                    //Punto para player2
                    if(ballX<0){
                        puntosPlayer2++;
                        textoPuntos2.setString(to_string(puntosPlayer2));
                        ball.setPosition(Vector2f(390.0f, 290.0f));
                        ballSpeed = Vector2f(5.0f, 3.0f);
                        rebotes=0; //Resetear rebotes
                    }
                    //Punto para player1
                    if(ballX>800.0f){
                        puntosPlayer1++;
                        textoPuntos1.setString(to_string(puntosPlayer1));
                        ball.setPosition(Vector2f(390.0f, 290.0f));
                        ballSpeed = Vector2f(-5.0f, 3.0f);
                        rebotes=0; //Resetear rebotes
                    }
                    //Condicion de victoria
                    if (puntosPlayer1 >= PUNTOS_VICTORIA || puntosPlayer2 >= PUNTOS_VICTORIA) {
                        ganadorGlobal = (puntosPlayer1 >= PUNTOS_VICTORIA) ? 1 : 2;

                        puntosPlayer1 = 0;
                        puntosPlayer2 = 0;
                        textoPuntos1.setString("0");
                        textoPuntos2.setString("0");
                        rebotes=0; //Resetear rebotes
                        player1.setPosition(Vector2f(50.0f, 250.0f));
                        player2.setPosition(Vector2f(735.0f, 250.0f));
                        ball.setPosition(Vector2f(390.0f, 290.0f));
                        ballSpeed = Vector2f(5.0f, 3.0f); // Resetear velocidad también

                        currentState = GameState::GAME_OVER;
                    }
                    
                    //Rebote con la paleta izquierda
                    Vector2f ballPos = ball.getPosition();
                    Vector2f player1Pos = player1.getPosition();
                    
                    if(ballPos.x <= player1Pos.x + 15.0f && 
                    ballPos.x + 20.0f >= player1Pos.x &&
                    ballPos.y <= player1Pos.y + 100.0f && 
                    ballPos.y + 20.0f >= player1Pos.y){
                        
                        ballSpeed.x = -ballSpeed.x;
                        ball.setPosition(Vector2f(player1Pos.x + 15.0f + 1.0f, ballPos.y));

                        rebotes++;
                        aplicarAceleracionPorRebote(ballSpeed, velocidadBase, incremento, rebotes, maxVelocidad);
                    }

                    //Rebote con la paleta derecha
                    Vector2f player2Pos = player2.getPosition();

                    if(ballPos.x <= player2Pos.x + 15.0f && 
                    ballPos.x + 20.0f >= player2Pos.x &&
                    ballPos.y <= player2Pos.y + 100.0f && 
                    ballPos.y + 20.0f >= player2Pos.y){
                        
                        ballSpeed.x = -ballSpeed.x;
                        ball.setPosition(Vector2f(player2Pos.x - 21.0f, ballPos.y));

                        rebotes++;
                        aplicarAceleracionPorRebote(ballSpeed, velocidadBase, incremento, rebotes, maxVelocidad);
                    }

                    //Limpiar pantalla
                    ventana.clear(Color::Black);
                    
                    //Dibujar paletas y pelota
                    ventana.draw(player1);
                    ventana.draw(player2);
                    ventana.draw(ball);
                    ventana.draw(textoPuntos1);
                    ventana.draw(textoPuntos2);
                    
                    //Dibujar linea central
                    for(int i=0;i<15;i++){
                        segment.setPosition(Vector2f(397.5f, static_cast<float>(i*40)));
                        ventana.draw(segment);
                    }

                    //Mostrar todo en pantalla
                    ventana.display();
                }
                break;
            
            case GameState::GAME_OVER:
                currentState = showGameOver(ventana, fuente, ganadorGlobal);
                break;

            case GameState::EXIT:
                ventana.close();
                break;
        }
    }
    
    return 0;
}