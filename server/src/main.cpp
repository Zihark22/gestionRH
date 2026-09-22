#include "../includes/server.hpp"

int main(int argc, char* argv[]) {

    Server server(argc, argv);
    server.start();

    // try { 
    //     Employee e("fzefezf");
    
    //     e.display();    
    // }
    // catch (const std::exception& e) {
    //     std::cout << "Erreur de création de l'employé !! \n\tErreur: " << e.what() << std::endl;
    // }  
    
    return 0;
}
