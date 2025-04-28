#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#ifndef _MACROS
#define src(x, y) (src.pixel[(x) + src.w * (y)])
#define dst(x, y) (dst.pixel[(x) + dst.w * (y)])
#endif

using namespace std;

struct pgm {
    string type;
    int w, h, bits;
    vector<uint8_t> pixel;
};

void lerComentario(ifstream &f) {
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void carregarImagem(pgm &src, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    lerComentario(f);
    f >> src.type;
    lerComentario(f);
    f >> src.w;
    lerComentario(f);
    f >> src.h;
    lerComentario(f);
    f >> src.bits;
    src.pixel.resize(src.w * src.h);
    if (src.type == "P5") {
        f.get();
        f.read(reinterpret_cast<char *>(src.pixel.data()), src.pixel.size());
    }
    if (src.type == "P2") {
        for (int y = 0; y < src.h; y++) {
            for (int x = 0; x < src.w; x++) {
                int v;
                f >> v;
                src(x, y) = v;
            }
        }
    }
    f.close();
}

void salvarImagem(pgm &src, const string &name) {
    ofstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    f << src.type << endl
      << src.w << ' ' << src.h << endl
      << src.bits << endl;
    if (src.type == "P5") {
        f.write((char *)&src.pixel[0], src.pixel.size());
    }
    if (src.type == "P2") {
        for (int y = 0; y < src.h; y++) {
            for (int x = 0; x < src.w; x++) {
                f << +src(x, y) << endl;
            }
        }
    }
    f.close();
}

void setHeader(pgm &src, string type, int w, int h, int bits) {
    src.type = type;
    src.w = w;
    src.h = h;
    src.bits = bits;
    src.pixel.clear();
    src.pixel.resize(w * h);
}

void clonarCabecalho(pgm src, pgm &dst) {
    dst.type = src.type;
    dst.bits = src.bits;
    dst.w = src.w;
    dst.h = src.h;
    dst.pixel.clear();
    dst.pixel.resize(src.w * src.h);
}

void clonarDados(pgm src, pgm &dst) {
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(x, y);
        }
    }
}

void inverter(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = 255 - src(x, y);
        }
    }
}

void transposta(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(y, x);
        }
    }
}

void flipVertical(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(x, (src.h - y - 1));
        }
    }
}

void flipHorizontal(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.w - x - 1), y);
        }
    }
}

void girar90(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src(y, (src.w - x - 1));
        }
    }
}

void girar180(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.w - x - 1), (src.h - y - 1));
        }
    }
}

void girar270(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = src((src.h - y - 1), x);
        }
    }
}

void binarizarArea(pgm src, pgm &dst, int min, int max) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) > min && src(x, y) < max) ? 255 : 0;
        }
    }
}

void destacarArea(pgm src, pgm &dst, int min, int max, int cor) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) > min && src(x, y) < max) ? cor : src(x, y);
        }
    }
}

void brilho(pgm src, pgm &dst, float num) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            int aux = src(x, y) * num;
            if (aux < 0)
                dst(x, y) = 0;
            else if (aux > 255)
                dst(x, y) = 255;
            else
                dst(x, y) = aux;
        }
    }
}

void correcaoGama(pgm src, pgm &dst, float gama) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            float aux = float(src(x, y)) / float(255);
            aux = pow(aux, gama) * 255;
            if (aux < 0)
                dst(x, y) = 0;
            else if (aux > 255)
                dst(x, y) = 255;
            else
                dst(x, y) = aux;
        }
    }
}

void ampliar(pgm src, pgm &dst) {
    setHeader(dst, src.type, src.w * 2, src.h * 2, 255);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(2 * x, 2 * y) = src(x, y);
            dst(2 * x + 1, 2 * y) = src(x, y);
            dst(2 * x, 2 * y + 1) = src(x, y);
            dst(2 * x + 1, 2 * y + 1) = src(x, y);
        }
    }
}

void reduzir(pgm src, pgm &dst) {
    setHeader(dst, src.type, src.w / 2, src.h / 2, 255);
    for (int y = 0; y < src.h / 2; y++) {
        for (int x = 0; x < src.w / 2; x++) {
            int aux = src(2 * x, 2 * y) +
                      src(2 * x + 1, 2 * y) +
                      src(2 * x, 2 * y + 1) +
                      src(2 * x + 1, 2 * y + 1);
            dst(x, y) = aux / 4;
        }
    }
}

void normalizar(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    int max = 0, min = 255;
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            if (src(x, y) > max) max = src(x, y);
            if (src(x, y) < min) min = src(x, y);
        }
    }
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) - min) * 255 / (max - min);
        }
    }
}

void tons(pgm src, pgm &dst, int nivel) {
    clonarCabecalho(src, dst);
    int aux = 256 / nivel;
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = (src(x, y) / aux) * aux;
        }
    }
    normalizar(dst, dst);
}

void plotHistograma(pgm src, pgm &dst) {
    vector<int> hist(256, 0);
    for (auto &i : src.pixel)
        hist[i]++;
    int max = 0;
    for (auto &i : hist)
        if (i > max) max = i;
    if (max == 0) return;
    for (auto &i : hist)
        i = i * 255 / max;
    setHeader(dst, "P5", 256, 256, 255);
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            dst(x, 255 - y) = (y < hist[x]) ? 0 : 255;
        }
    }
}

void histograma(pgm src) {
    vector<int> hist(256, 0);
    ofstream f("histograma.txt", ios::out);
    for (auto &i : src.pixel)
        hist[i]++;
    for (int i = 0; i < 256; i++)
        f << i << " " << hist[i] << endl;
    f.close();
}

void equalizarHistograma(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    vector<int> hist(256, 0);
    vector<float> fda(256, 0.0), sk(256, 0.0);
    for (auto &i : src.pixel) {
        hist[i]++;
    }
    for (int i = 0; i < 256; i++) {
        fda[i] = float(hist[i]) / float(src.w * src.h);
    }
    float soma = 0;
    for (int i = 0; i < 256; i++) {
        soma += fda[i];
        sk[i] = soma * 255;
    }
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y) = sk[src(x, y)];
        }
    }
}

void equalizarHistogramaLocal(pgm src, pgm &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y += range) {
        if (y + range > src.h) continue;
        for (int x = 0; x < src.w; x += range) {
            if (x + range > src.w) continue;
            vector<int> hist(256, 0);
            vector<float> fda(256, 0.0), sk(256, 0.0);
            for (int yy = y; yy < y + range; yy++) {
                for (int xx = x; xx < x + range; xx++) {
                    hist[src(xx, yy)]++;
                }
            }
            for (int i = 0; i < 256; i++) {
                fda[i] = float(hist[i]) / float(range * range);
            }
            float soma = 0;
            for (int i = 0; i < 256; i++) {
                soma += fda[i];
                sk[i] = soma * 255;
            }
            for (int yy = y; yy < y + range; yy++) {
                for (int xx = x; xx < x + range; xx++) {
                    dst(xx, yy) = sk[src(xx, yy)];
                }
            }
        }
    }
}

void media(pgm src, pgm &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            int pixel = 0, count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    pixel += src(xx, yy);
                    count++;
                }
            }
            dst(x, y) = pixel * (1.0f / count);
        }
    }
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quicksort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

void mediana(pgm src, pgm &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            vector<int> pixel;
            int count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    pixel.push_back(src(xx, yy));
                    count++;
                }
            }
            quicksort(pixel, 0, pixel.size() - 1);
            dst(x, y) = pixel.at(count / 2 + 1);
        }
    }
}

void highBoost(pgm src, pgm &dst, int range, float k) {
    clonarCabecalho(src, dst);
    pgm blur;
    media(src, blur, range);
    vector<int> masc;
    masc.resize(src.pixel.size());
    for (uint32_t i = 0; i < src.pixel.size(); i++) {
        masc[i] = src.pixel[i] - blur.pixel[i];
        int aux = src.pixel[i] + (masc[i] * k);
        if (aux > 255) dst.pixel[i] = 255;
        else if (aux < 0) dst.pixel[i] = 0;
        else dst.pixel[i] = aux;
    }
}

void laplaciano(pgm src, pgm &dst) {
    clonarCabecalho(src, dst);
    clonarDados(src, dst);
    vector<int> masc{1, 1, 1, 1, -8, 1, 1, 1, 1};
    for (int y = 1; y < src.h - 1; y++) {
        for (int x = 1; x < src.w - 1; x++) {
            int pos_masc = 0;
            int cont_masc = 0;
            for (int yy = -1; yy <= 1; yy++) {
                for (int xx = -1; xx <= 1; xx++) {
                    pos_masc += src(x + xx, y + yy) * masc[cont_masc];
                    cont_masc++;
                }
            }
            pos_masc /= 9;
            dst(x, y) = pos_masc;
        }
    }
}
