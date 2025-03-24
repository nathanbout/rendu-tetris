#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <thread>
#include <chrono>


/// Pour compiler : 
/// g++ -o main main.cpp  -lsfml-graphics -lsfml-window -lsfml-system

sf::Color grey(125, 125, 125);
sf::VertexArray boardDesign();
sf::RenderWindow window(sf::VideoMode(1500, 900), "Tetris" );
int lap = 500;

class Piece {

    protected:
        std::vector<std::pair<int,int>> coords_;
    public:
        Piece(std::vector<std::pair<int,int>> coords): coords_{coords}{};

        int firstcoord(int i){
            return this->coords_[i].first;
        }
        int seccoord(int i){
            return this->coords_[i].second;
        }

        int size(){
            return this->coords_.size();
        }
    
        void move(int dir){
            auto fake = coords_;
            bool flag = true;
            if(dir == 1){
                for(int i=0; i<coords_.size(); i++){
                    fake[i].first+=1;
                    if(fake[i].first==10){
                        flag = false;
                    }
                }
            }
            if(dir == 0){
                for(int i=0; i<coords_.size(); i++){
                    fake[i].first+=-1;
                    if(fake[i].first==-1){
                        flag = false;
                    }
                }
            }
            if(flag){
                coords_=fake;
            }
            
        }

        void movelow(){
            for(int i=0; i<coords_.size(); i++){
                coords_[i].second+=1;
            }
        }

};

class T : public Piece
{
    public:
        T(): Piece({{4,0},{4,1},{4,2},{5,1}}){};
        


};

class Board {
    protected :
        std::vector<int> tab;

    public:
        Board(): tab(200, 0) {}

        int operator[](int i){
            return tab[i];
        }

        void setvalue(int i, int a){
            this->tab[i] = a;
        }

        void print(){
            for(int i = 0; i<200; i++){
                std::cout << this->tab[i] << std::endl;
            }
        }
    
        void showpiece(){
            for(int i=0; i<200; i++){
                if(tab[i] == 0){
                    int x = i%10;
                    int y = (i-x)/10;
                    sf::RectangleShape rect({40.f, 40.f});
                    rect.setPosition({550.f+x*41.f, 50.f+y*41.f});
                    rect.setFillColor(sf::Color::Black);
                    window.draw(rect);

                }
                if(tab[i] == 1){
                    int x = i%10;
                    int y = (i-x)/10;
                    sf::RectangleShape rect({40.f, 40.f});
                    rect.setPosition({550.f+x*41.f, 50.f+y*41.f});
                    rect.setFillColor(sf::Color::Blue);
                    window.draw(rect);
                    
                }
                if(tab[i] == -1){
                    int x = i%10;
                    int y = (i-x)/10;
                    sf::RectangleShape rect({40.f, 40.f});
                    rect.setPosition({550.f+x*41.f, 50.f+y*41.f});
                    rect.setFillColor(sf::Color::White);
                    window.draw(rect);
                    
                }
            }

        }

        void fillpiece(Piece piece){
            for(int i=0; i<piece.size(); i++){
                this->tab[piece.firstcoord(i)+piece.seccoord(i)*10]=1;
            }
        }

        void erase(){
            for(int i=0; i<200; i++){
                if(tab[i] != 0 & tab[i] != -1){
                    tab[i]=0;
                }
            }
        }

        bool verify(int x, int y){
            if(y>19){
                return false;
            }
            if(tab[y*10+x]!=0){
                return false;
            }
            return true;
        }
        
        
};

bool update(Piece piece, Board board){
    auto fake = piece ;
    fake.movelow();

    int i=0;
    bool flag=true;
    while(flag & i<fake.size()){
        flag = board.verify(fake.firstcoord(i), fake.seccoord(i));
        i+=1;
    }

    if(flag){
        return false;
    }
    
    return true;
    
     
}

int main(){
    window.setPosition({200,50});
    sf::VertexArray board = boardDesign();

    Board plateau;
    T piece1;

 

    while(window.isOpen()){

        sf::Event event;

        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed){
                window.close();
            }

            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Right){
                    piece1.move(1);
                }
                else if(event.key.code == sf::Keyboard::Left){
                    piece1.move(0);
                }

            }
        }


        plateau.fillpiece(piece1);
        window.clear(sf::Color::Black);
        window.draw(board);
        plateau.showpiece();
        window.display();
        plateau.erase();
        std::this_thread::sleep_for(std::chrono::milliseconds(lap));
        bool nouv = update(piece1, plateau);
        if(nouv){
            for(int i=0; i<piece1.size(); i++){
                plateau.setvalue(piece1.firstcoord(i)+piece1.seccoord(i)*10, -1);
            }
            T piecenew;
            piece1=piecenew;
            
        }
        else{
            piece1.movelow();
        }
        

    }

}

sf::VertexArray boardDesign(){
    sf::VertexArray board(sf::PrimitiveType::LinesStrip, 65);
    board[0].position = sf::Vector2f(550.f, 50.f);
    board[1].position = sf::Vector2f(960.f, 50.f);
    board[2].position = sf::Vector2f(960.f, 871.f);
    board[3].position = sf::Vector2f(550.f, 871.f);
    board[4].position = sf::Vector2f(550.f, 50.f);

    int i;
    for( i=0; i<9; i+=2){
        board[2*i+5].position = sf::Vector2f(550.f + (i+1)*41.f, 50.f);
        board[2*i+6].position = sf::Vector2f(550.f + (i+1)*41.f, 871.f);
        board[2*i+7].position = sf::Vector2f(550.f + (i+2)*41.f, 871.f);
        board[2*i+8].position = sf::Vector2f(550.f + (i+2)*41.f, 50.f);
    }

    for( i=0; i<10; i++){
        board[4*i+25].position = sf::Vector2f(960.f, 50.f + (2*i+1)*41.f);
        board[4*i+26].position = sf::Vector2f(550.f, 50.f + (2*i+1)*41.f);
        board[4*i+27].position = sf::Vector2f(550.f, 50.f + (2*i+2)*41.f);
        board[4*i+28].position = sf::Vector2f(960.f, 50.f + (2*i+2)*41.f);
    }

    for(i=0; i<65; i++){
        board[i].color = grey;
    }

    return board;
}