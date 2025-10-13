#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

int main(){
    //Crear la ventana del juego
    RenderWindow ventana(VideoMode({800, 600}), "Pong - SFML 3.0.2");
    ventana.setFramerateLimit(60);

    //Variables de Puntuacion
    int puntosPlayer1 = 0;
    int puntosPlayer2 = 0;
    const int PUNTOS_VICTORIA = 5;

    //Sistema de texto

    Font fuente;

    if(!fuente.openFromFile("assets/DejaVuSans.ttf")){
        //si no encuentra la fuente
        cout<<"Error al cargar la fuente"<<endl;
        return -1;
    }  

    Text textoPuntos1(fuente,"0",50);
    Text textoPuntos2(fuente,"0",50);

    //Texto player1
    textoPuntos1.setFillColor(Color::White);
    textoPuntos1.setPosition({300.0f, 20.0f});
    //Texto player2
    textoPuntos2.setFillColor(Color::White);
    textoPuntos2.setPosition({480.0f, 20.0f});


    //Paleta 1 (izquierda)
    RectangleShape player1;
    player1.setSize({15.0f, 100.0f});         //Ancho, alto
    player1.setPosition({50.0f, 250.0f});     //Posicion x,y
    player1.setFillColor(Color::White);       //Color

    //Paleta 2 (derecha)
    RectangleShape player2;
    player2.setSize({15.0f, 100.0f});         //Ancho, alto
    player2.setPosition({735.0f, 250.0f});    //Posicion x,y
    player2.setFillColor(Color::White);       //Color

    //Pelota
    CircleShape ball(10.0f);                  //Radio de 10 px
    ball.setPosition({390.0f, 290.0f});       //Posicion en el centro
    ball.setFillColor(Color::White);          //Color

    //Linea Central
    RectangleShape segment;
    segment.setSize({5.0f, 20.0f});           //Ancho, alto
    segment.setFillColor(Color::White);       //Color

    //Variable de velocidad
    float paletaSpeed = 8.0f;                 //Velocidad de las paletas
    Vector2f ballSpeed({5.0f, 3.0f});      // Velocidad inicial (x, y)

    //Bucle principal del juego
    while(ventana.isOpen()){
        //Manejo de eventos (cerrar)
        while(auto event = ventana.pollEvent()){
            if(event->is<Event::Closed>()){
                ventana.close();
            }
        }

        //Controles Player1 (W-S)
        if(Keyboard::isKeyPressed(Keyboard::Key::W)){
            player1.move({0.0f, -paletaSpeed});     //Mover hacia arriba

            //Limitar borde superiormente
            if(player1.getPosition().y < 0.0f){
                player1.setPosition({50.0f, 0.0f});
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::S)){
            player1.move({0.0f, paletaSpeed});     //Mover hacia abajo

            //Limitar borde inferiormente
            if(player1.getPosition().y > 500.0f){
                player1.setPosition({50.0f, 500.0f});
            }
        }
        //Controles Player2 (Flechas)
        if(Keyboard::isKeyPressed(Keyboard::Key::Up)){
            player2.move({0.0f, -paletaSpeed});     //Mover hacia arriba

            //Limitar borde superiormente
            if(player2.getPosition().y < 0.0f){
                player2.setPosition({735.0f, 0.0f});
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::Down)){
            player2.move({0.0f, paletaSpeed});     //Mover hacia abajo

            //Limitar borde inferiormente
            if(player2.getPosition().y > 500.0f){
                player2.setPosition({735.0f, 500.0f});
            }
        }

        //Limites de la Pelota
        ball.move(ballSpeed);
        float ballTop = ball.getPosition().y;
        float ballBottom = ballTop + 20.0f;

        //Rebote en el techo (y<0)
        if (ballTop < 0.0f){
            ballSpeed.y = -ballSpeed.y;                       //invertir y 
            ball.setPosition({ball.getPosition().x,0.0f});    //Correccion de posicion
        }
        //Rebote en el suelo   
        if (ballBottom > 600.0f){
            ballSpeed.y = -ballSpeed.y;                       //invertir y 
            ball.setPosition({ball.getPosition().x,580.0f});  //Correccion de posicion
        }
        //Detectar anotacion
        float ballX = ball.getPosition().x;
        //Punto para player2
        if(ballX<0){
            puntosPlayer2++;
            textoPuntos2.setString(to_string(puntosPlayer2));
            //Reiniciar pelota
            ball.setPosition({390.0f,290.0f});
            ballSpeed = Vector2f({5.0f, 3.0f});   //velocidad inicial a la derecha
        }
        //Punto para player1
        if(ballX>800.0f){
            puntosPlayer1++;
            textoPuntos1.setString(to_string(puntosPlayer1));
            //Reiniciar pelota
            ball.setPosition({390.0f,290.0f});
            ballSpeed = Vector2f({-5.0f, 3.0f});   //velocidad inicial a la izquierda
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
            
            ballSpeed.x = -ballSpeed.x;                                        //invertir x
            ball.setPosition({player1Pos.x + 15.0f + 1.0f, ballPos.y});      //correccion de posicion
        }

        //Rebote con la paleta derecha
        Vector2f player2Pos = player2.getPosition();

        if(ballPos.x <= player2Pos.x + 15.0f && 
           ballPos.x + 20.0f >= player2Pos.x &&
           ballPos.y <= player2Pos.y + 100.0f && 
           ballPos.y + 20.0f >= player2Pos.y){
            
            ballSpeed.x = -ballSpeed.x;
            ball.setPosition({player2Pos.x - 21.0f, ballPos.y});
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
            segment.setPosition({397.5f, static_cast<float>(i*40)});
            ventana.draw(segment);
        }

        //Mostrar todo en pantalla
        ventana.display();
    }
    
    return 0;
}