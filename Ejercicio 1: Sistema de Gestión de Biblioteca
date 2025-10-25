// biblioteca.cpp
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits>

class Libro {
    std::string titulo_, autor_, isbn_;
    bool disponible_;
public:
    Libro() : disponible_(true) {}
    Libro(std::string t, std::string a, std::string i, bool d=true)
        : titulo_(std::move(t)), autor_(std::move(a)), isbn_(std::move(i)), disponible_(d) {}

    const std::string& titulo() const { return titulo_; }
    const std::string& autor()  const { return autor_;  }
    const std::string& isbn()   const { return isbn_;   }
    bool disponible() const { return disponible_; }
    void setDisponible(bool d){ disponible_ = d; }
};

class Biblioteca {
    std::vector<Libro> libros_;
    int buscarIndicePorISBN(const std::string& isbn) const {
        for (size_t i = 0; i < libros_.size(); ++i)
            if (libros_[i].isbn() == isbn) return static_cast<int>(i);
        return -1;
    }
public:
    bool agregar(const Libro& l){
        if (buscarIndicePorISBN(l.isbn()) != -1) return false;
        libros_.push_back(l);
        return true;
    }
    bool eliminarPorISBN(const std::string& isbn){
        int idx = buscarIndicePorISBN(isbn);
        if (idx == -1) return false;
        libros_.erase(libros_.begin() + idx);
        return true;
    }
    std::optional<Libro> buscarPorTitulo(const std::string& t) const {
        for (const auto& l : libros_) if (l.titulo() == t) return l;
        return std::nullopt;
    }
    std::vector<Libro> buscarPorAutor(const std::string& a) const {
        std::vector<Libro> r; for (const auto& l : libros_) if (l.autor() == a) r.push_back(l); return r;
    }
    std::vector<Libro> disponibles() const {
        std::vector<Libro> r; for (const auto& l : libros_) if (l.disponible()) r.push_back(l); return r;
    }
    const std::vector<Libro>& todos() const { return libros_; }
};

static void imprimir(const Libro& l){
    std::cout << "Titulo: " << l.titulo()
              << " | Autor: " << l.autor()
              << " | ISBN: "  << l.isbn()
              << " | Disponible: " << (l.disponible()? "Si":"No") << '\n';
}

static int menu(){
    std::cout << "\n=== Biblioteca ===\n"
              << "1) Agregar libro\n"
              << "2) Eliminar libro (ISBN)\n"
              << "3) Buscar por titulo\n"
              << "4) Buscar por autor\n"
              << "5) Mostrar disponibles\n"
              << "6) Mostrar todos\n"
              << "0) Salir\n> ";
    int op;
    if (!(std::cin >> op)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return op;
}

int main(){
    Biblioteca b;
    // Datos demo
    b.agregar(Libro("Cien anios de soledad","Gabo","ISBN-001",true));
    b.agregar(Libro("Rayuela","Cortazar","ISBN-002",true));
    b.agregar(Libro("El coronel no tiene quien le escriba","Gabo","ISBN-003",false));

    while (true){
        int op = menu();
        if (op == 0) break;
        if (op == 1){
            std::string t,a,i; char d='s';
            std::cout<<"Titulo: "; std::getline(std::cin,t);
            std::cout<<"Autor: "; std::getline(std::cin,a);
            std::cout<<"ISBN: "; std::getline(std::cin,i);
            std::cout<<"Disponible? (s/n): "; std::cin>>d; 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << (b.agregar(Libro(t,a,i,(d=='s'||d=='S'))) ? "Agregado.\n":"ERROR: ISBN duplicado.\n");
        } else if (op == 2){
            std::string i; std::cout<<"ISBN a eliminar: "; std::getline(std::cin,i);
            std::cout << (b.eliminarPorISBN(i) ? "Eliminado.\n":"No existe.\n");
        } else if (op == 3){
            std::string t; std::cout<<"Titulo: "; std::getline(std::cin,t);
            auto r=b.buscarPorTitulo(t); if(r) imprimir(*r); else std::cout<<"No encontrado.\n";
        } else if (op == 4){
            std::string a; std::cout<<"Autor: "; std::getline(std::cin,a);
            auto v=b.buscarPorAutor(a); if(v.empty()) std::cout<<"Sin resultados.\n"; else for(auto& l:v) imprimir(l);
        } else if (op == 5){
            auto v=b.disponibles(); if(v.empty()) std::cout<<"No hay disponibles.\n"; else for(auto& l:v) imprimir(l);
        } else if (op == 6){
            for (auto& l: b.todos()) imprimir(l);
        } else {
            std::cout<<"Opcion invalida.\n";
        }
    }
    return 0;
}
