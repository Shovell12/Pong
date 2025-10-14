#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

// Enum debe estar después de los includes y antes de los using namespace
enum class GameState{
    MENU,
    PLAYING,
    EXIT
};

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

int main(){
    //Crear la ventana del juego
    RenderWindow ventana(VideoMode({800, 600}), "Pong - SFML 3.0.2");
    ventana.setFramerateLimit(60);
    
    //Estado del juego
    GameState currentState = GameState::MENU;

    //Variables de Puntuacion
    int puntosPlayer1 = 0;
    int puntosPlayer2 = 0;
    const int PUNTOS_VICTORIA = 5;

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
                    }
                    //Punto para player1
                    if(ballX>800.0f){
                        puntosPlayer1++;
                        textoPuntos1.setString(to_string(puntosPlayer1));
                        ball.setPosition(Vector2f(390.0f, 290.0f));
                        ballSpeed = Vector2f(-5.0f, 3.0f);
                    }
                    //Condicion de victoria
                    if (puntosPlayer1 >= PUNTOS_VICTORIA || puntosPlayer2 >= PUNTOS_VICTORIA) {
                        puntosPlayer1 = 0;
                        puntosPlayer2 = 0;
                        textoPuntos1.setString("0");
                        textoPuntos2.setString("0");
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
                    }

                    //Rebote con la paleta derecha
                    Vector2f player2Pos = player2.getPosition();

                    if(ballPos.x <= player2Pos.x + 15.0f && 
                    ballPos.x + 20.0f >= player2Pos.x &&
                    ballPos.y <= player2Pos.y + 100.0f && 
                    ballPos.y + 20.0f >= player2Pos.y){
                        
                        ballSpeed.x = -ballSpeed.x;
                        ball.setPosition(Vector2f(player2Pos.x - 21.0f, ballPos.y));
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
            
            case GameState::EXIT:
                ventana.close();
                break;
        }
    }
    
    return 0;
}