// alquiler.cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>

class Vehiculo {
protected:
    std::string marca_, modelo_, placa_;
    bool disponible_ = true;
public:
    Vehiculo() = default;
    Vehiculo(std::string ma, std::string mo, std::string pl)
        : marca_(std::move(ma)), modelo_(std::move(mo)), placa_(std::move(pl)) {}
    virtual ~Vehiculo() = default;

    const std::string& marca()  const { return marca_;  }
    const std::string& modelo() const { return modelo_; }
    const std::string& placa()  const { return placa_;  }
    bool disponible() const { return disponible_; }
    void setDisponible(bool d) { disponible_ = d; }

    virtual void mostrarInformacion(std::ostream& os) const {
        os << "Vehiculo | Marca: " << marca_
           << " | Modelo: " << modelo_
           << " | Placa: "  << placa_
           << " | Disponible: " << (disponible_ ? "Si" : "No");
    }
};
static std::ostream& operator<<(std::ostream& os, const Vehiculo& v){ v.mostrarInformacion(os); return os; }

class Auto : public Vehiculo {
    int capacidad_;
public:
    Auto() = default;
    Auto(std::string ma, std::string mo, std::string pl, int cap)
        : Vehiculo(std::move(ma), std::move(mo), std::move(pl)), capacidad_(cap) {}
    void mostrarInformacion(std::ostream& os) const override {
        os << "Auto | Marca: " << marca_
           << " | Modelo: "   << modelo_
           << " | Placa: "    << placa_
           << " | Capacidad: "<< capacidad_
           << " | Disponible: " << (disponible_ ? "Si" : "No");
    }
};

class Bicicleta : public Vehiculo {
    std::string tipo_;
public:
    Bicicleta() = default;
    Bicicleta(std::string ma, std::string mo, std::string pl, std::string tipo)
        : Vehiculo(std::move(ma), std::move(mo), std::move(pl)), tipo_(std::move(tipo)) {}
    void mostrarInformacion(std::ostream& os) const override {
        os << "Bicicleta | Marca: " << marca_
           << " | Modelo: " << modelo_
           << " | Placa: "  << placa_
           << " | Tipo: "   << tipo_
           << " | Disponible: " << (disponible_ ? "Si" : "No");
    }
};

class SistemaAlquiler {
    std::vector<std::unique_ptr<Vehiculo>> inv_;
    int idxPorPlaca(const std::string& p) const {
        for (size_t i = 0; i < inv_.size(); ++i)
            if (inv_[i]->placa() == p) return static_cast<int>(i);
        return -1;
    }
public:
    template<class T, class... Args>
    void registrar(Args&&... args) {
        inv_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    bool alquilar(const std::string& p){
        int i = idxPorPlaca(p); if (i == -1) return false;
        if (!inv_[i]->disponible()) return false;
        inv_[i]->setDisponible(false); return true;
    }
    bool devolver(const std::string& p){
        int i = idxPorPlaca(p); if (i == -1) return false;
        inv_[i]->setDisponible(true); return true;
    }
    std::vector<const Vehiculo*> disponibles() const {
        std::vector<const Vehiculo*> r;
        for (const auto& x : inv_) if (x->disponible()) r.push_back(x.get());
        return r;
    }
    const std::vector<std::unique_ptr<Vehiculo>>& todos() const { return inv_; }
};

static void listar(const SistemaAlquiler& s, bool soloDisp=false){
    if (soloDisp) {
        auto v = s.disponibles();
        if (v.empty()) { std::cout<<"No hay disponibles.\n"; return; }
        for (auto* p : v) std::cout << *p << '\n';
    } else {
        for (const auto& p : s.todos()) std::cout << *p.get() << '\n';
    }
}

static int menu(){
    std::cout << "\n=== Alquiler de Vehiculos ===\n"
              << "1) Registrar Auto\n"
              << "2) Registrar Bicicleta\n"
              << "3) Alquilar (por placa)\n"
              << "4) Devolver (por placa)\n"
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
    SistemaAlquiler s;
    // Datos demo
    s.registrar<Auto>("Toyota","Corolla","ABC123",5);
    s.registrar<Bicicleta>("GW","KONA","BICI01","montana");

    while (true){
        int op = menu();
        if (op == 0) break;
        if (op == 1){
            std::string m,mo,p; int cap;
            std::cout<<"Marca: "; std::getline(std::cin,m);
            std::cout<<"Modelo: "; std::getline(std::cin,mo);
            std::cout<<"Placa: ";  std::getline(std::cin,p);
            std::cout<<"Capacidad pasajeros: "; std::cin>>cap;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            s.registrar<Auto>(m,mo,p,cap);
            std::cout<<"Auto registrado.\n";
        } else if (op == 2){
            std::string m,mo,p,t;
            std::cout<<"Marca: "; std::getline(std::cin,m);
            std::cout<<"Modelo: "; std::getline(std::cin,mo);
            std::cout<<"Placa: ";  std::getline(std::cin,p);
            std::cout<<"Tipo (ruta/montana/urbana): "; std::getline(std::cin,t);
            s.registrar<Bicicleta>(m,mo,p,t);
            std::cout<<"Bicicleta registrada.\n";
        } else if (op == 3){
            std::string p; std::cout<<"Placa a alquilar: "; std::getline(std::cin,p);
            std::cout<<(s.alquilar(p)? "Alquiler OK.\n":"No existe o no disponible.\n");
        } else if (op == 4){
            std::string p; std::cout<<"Placa a devolver: "; std::getline(std::cin,p);
            std::cout<<(s.devolver(p)? "Devuelto.\n":"No existe.\n");
        } else if (op == 5) listar(s,true);
        else if (op == 6) listar(s,false);
        else std::cout<<"Opcion invalida.\n";
    }
    return 0;
}
