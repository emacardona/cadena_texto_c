#include <iostream>
#include <string>
#include <vector>
#include <cwctype> 
#include <io.h>      
#include <fcntl.h>

using namespace std;


bool esFuerte(wchar_t c) {
    return wstring(L"aeoáéóAEOÁÉÓ").find(c) != wstring::npos;
}

bool esDebil(wchar_t c) {
    return wstring(L"iuüIUÜ").find(c) != wstring::npos;
}

bool esTildada(wchar_t c) {
    return wstring(L"íúÍÚ").find(c) != wstring::npos;
}

int main() {
    
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wchar_t opcion = L's';

    while (opcion == L's' || opcion == L'S') {
        
       
        wcout << L"\n\n"; 
        wcout << L"****************************************" << endl;
        wcout << L"********* ANALIZADOR DE TEXTO *********" << endl;
        wcout << L"****************************************" << endl;
        wcout << L"Escribe tu frase y presiona ENTER:" << endl;

        wstring texto;
 
        getline(wcin, texto);

        
        if (texto.empty()) {
            getline(wcin, texto);
        }

        
        int palabras = 0, espacios = 0, saltos = 0;
        int vocalesMin = 0, vocalesMay = 0, numeros = 0, signos = 0;
        
        vector<wstring> listTildes, listDiptongos, listHiatos;
        wstring palabraActual = L"";
        
        texto += L" "; // 

        
        for (size_t i = 0; i < texto.length(); i++) {
            wchar_t letra = texto[i];

           
            if (iswspace(letra)) {
                espacios++;
                if (letra == L'\n') saltos++;
            }
            else if (iswdigit(letra)) numeros++;
            else if (iswpunct(letra)) signos++;

           
            if (wstring(L"aeiou").find(letra) != wstring::npos) vocalesMin++;
            if (wstring(L"AEIOU").find(letra) != wstring::npos) vocalesMay++;

            
            if (iswalpha(letra)) {
                palabraActual += letra;
            } 
            else if (palabraActual.length() > 0) {
                palabras++;
                
        
                bool tilde = false, diptongo = false, hiato = false;

                for (size_t k = 0; k < palabraActual.length(); k++) {
                    wchar_t actual = palabraActual[k];
                    
                    if (wstring(L"áéíóúÁÉÍÓÚüÜ").find(actual) != wstring::npos) tilde = true;

                    if (k < palabraActual.length() - 1) {
                        wchar_t sig = palabraActual[k+1];
                        
                        // Diptongo
                        if ((esDebil(actual) && esDebil(sig)) || 
                            (esFuerte(actual) && esDebil(sig)) || 
                            (esDebil(actual) && esFuerte(sig))) diptongo = true;

                        // Hiato
                        if ((esFuerte(actual) && esFuerte(sig)) || 
                            (esFuerte(actual) && esTildada(sig)) || 
                            (esTildada(actual) && esFuerte(sig))) hiato = true;
                    }
                }

                if (tilde) listTildes.push_back(palabraActual);
                if (diptongo) listDiptongos.push_back(palabraActual);
                if (hiato) listHiatos.push_back(palabraActual);

                palabraActual = L""; 
            }
        }
        
        if (espacios > 0) espacios--;

        
        wcout << endl << L"=== RESULTADOS ===" << endl;
        wcout << L"Total Palabras: " << palabras << endl;
        wcout << L"Espacios:       " << espacios << endl;
        wcout << L"Vocales Min:    " << vocalesMin << endl;
        wcout << L"Vocales May:    " << vocalesMay << endl;
        wcout << L"Numeros:        " << numeros << endl;
        wcout << L"Signos:         " << signos << endl;
        wcout << endl;

        wcout << L"Con Tilde: ";
        for (const auto& p : listTildes) wcout << p << L", ";
        wcout << endl;

        wcout << L"Con Diptongo: ";
        for (const auto& p : listDiptongos) wcout << p << L", ";
        wcout << endl;

        wcout << L"Con Hiato: ";
        for (const auto& p : listHiatos) wcout << p << L", ";
        wcout << endl << endl;

        wcout << L"¿Otra vez? (s/n): ";
        wcin >> opcion;
        
      
        wcin.ignore();
    }

    return 0;
}