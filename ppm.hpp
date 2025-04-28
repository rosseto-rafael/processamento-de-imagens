#pragma once

#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "pgm.hpp"

using namespace std;

struct rgb {
    uint8_t r, g, b;
};

struct ppm {
    string type;
    int w, h, bits;
    vector<rgb> pixel;
};

void carregarImagem(ppm &src, const string &name) {
    ifstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Could not open file: " << name << endl;
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
    if (src.type == "P6") {
        f.get();
        f.read(reinterpret_cast<char *>(src.pixel.data()), src.pixel.size() * 3);
    } else if (src.type == "P3") {
        for (uint32_t i = 0; i < src.pixel.size(); i++) {
            int v;
            f >> v;
            src.pixel[i].r = v;
            f >> v;
            src.pixel[i].g = v;
            f >> v;
            src.pixel[i].b = v;
        }
    }
    f.close();
}

void salvarImagem(ppm &src, const string &name) {
    ofstream f(name.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Erro ao abrir o arquivo " << name << endl;
        return;
    }
    f << src.type << endl
      << src.w << ' ' << src.h << endl
      << src.bits << endl;
    if (src.type == "P6") {
        f.write((char *)&src.pixel[0], src.pixel.size() * 3);
    } else if (src.type == "P3") {
        for (int i = 0; i < src.h; i++) {
            for (int j = 0; j < src.w; j++) {
                f << +src(j, i).r << ' ';
                f << +src(j, i).g << ' ';
                f << +src(j, i).b << endl;
            }
        }
        f.close();
    }
}

void printHeader(ppm src) {
    cout << src.type << endl
         << src.w << ' ' << src.h << endl
         << src.bits << endl;
}

void printData(ppm src) {
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            cout << +src(j, i).r << ' ';
            cout << +src(j, i).g << ' ';
            cout << +src(j, i).b << endl;
        }
    }
}

void setHeader(ppm &src, string type, int w, int h, int bits) {
    src.type = type;
    src.w = w;
    src.h = h;
    src.bits = bits;
    src.pixel.clear();
    src.pixel.resize(w * h);
}

void clonarCabecalho(ppm src, ppm &dst) {
    dst.type = src.type;
    dst.bits = src.bits;
    dst.w = src.w;
    dst.h = src.h;
    dst.pixel.clear();
    dst.pixel.resize(src.w * src.h);
}

void clonarDados(ppm src, ppm &dst) {
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src(j, i);
        }
    }
}

void inverter(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i).r = 255 - src(j, i).r;
            dst(j, i).g = 255 - src(j, i).g;
            dst(j, i).b = 255 - src(j, i).b;
        }
    }
}

void transposta(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(i, j) = src(j, i);
        }
    }
}

void flipHorizontal(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src((src.h - j - 1), i);
        }
    }
}

void flipVertical(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src(j, (src.w - i - 1));
        }
    }
}

void girar90(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src(i, (src.h - j - 1));
        }
    }
}

void girar180(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src((src.w - j - 1), (src.h - i - 1));
        }
    }
}

void girar270(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            dst(j, i) = src((src.w - i - 1), j);
        }
    }
}

void plotHistograma(ppm src, ppm &dst) {
    vector<int> R(256, 0), G(256, 0), B(256, 0);
    int max = 0;
    for (auto &i : src.pixel) {
        R[i.r]++;
        G[i.g]++;
        B[i.b]++;
    }
    for (int i = 0; i < 256; i++) {
        if (R[i] > max) max = R[i];
        if (G[i] > max) max = G[i];
        if (B[i] > max) max = B[i];
    }
    for (int i = 0; i < 256; i++) {
        R[i] = R[i] * 255 / max;
        G[i] = G[i] * 255 / max;
        B[i] = B[i] * 255 / max;
    }
    setHeader(dst, "P6", 256, 256, 255);
    for (auto &i : dst.pixel) {
        i = {0, 0, 0};
    }
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            if (y < R[x])
                dst(x, (255 - y)).r += 255;
            if (y < G[x])
                dst(x, (255 - y)).g += 255;
            if (y < B[x])
                dst(x, (255 - y)).b += 255;
        }
    }
}

void equalizarHistograma(ppm src, ppm &dst) {
    clonarCabecalho(src, dst);
    vector<int> hist_r(256, 0), hist_g(256, 0), hist_b(256, 0);
    vector<float> fda_r(256, 0.0), fda_g(256, 0.0), fda_b(256, 0.0);
    vector<float> sk_r(256, 0.0), sk_g(256, 0.0), sk_b(256, 0.0);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            hist_r[src(x, y).r]++;
            hist_g[src(x, y).g]++;
            hist_b[src(x, y).b]++;
        }
    }
    for (int i = 0; i < 256; i++) {
        fda_r[i] = float(hist_r[i]) / float(src.w * src.h);
        fda_g[i] = float(hist_g[i]) / float(src.w * src.h);
        fda_b[i] = float(hist_b[i]) / float(src.w * src.h);
    }
    float soma_r = 0, soma_g = 0, soma_b = 0;
    for (int i = 0; i < 256; i++) {
        soma_r += fda_r[i];
        soma_g += fda_g[i];
        soma_b += fda_b[i];
        sk_r[i] = soma_r * 255;
        sk_g[i] = soma_g * 255;
        sk_b[i] = soma_b * 255;
    }
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            dst(x, y).r = sk_r[src(x, y).r];
            dst(x, y).g = sk_g[src(x, y).g];
            dst(x, y).b = sk_b[src(x, y).b];
        }
    }
}

void media(ppm src, ppm &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            int R = 0, G = 0, B = 0;
            int count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    R += src(xx, yy).r;
                    G += src(xx, yy).g;
                    B += src(xx, yy).b;
                    count++;
                }
            }
            dst(x, y).r = R * (1.0f / count);
            dst(x, y).g = G * (1.0f / count);
            dst(x, y).b = B * (1.0f / count);
        }
    }
}

/* void mediana(ppm src, ppm &dst, int range) {
    clonarCabecalho(src, dst);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            vector<int> R, G, B;
            int count = 0;
            for (int yy = y - range; yy <= y + range; yy++) {
                for (int xx = x - range; xx <= x + range; xx++) {
                    if (yy < 0 || xx < 0 || yy >= src.h || xx >= src.w)
                        continue;
                    R.push_back(src(xx, yy).r);
                    G.push_back(src(xx, yy).g);
                    B.push_back(src(xx, yy).b);
                    count++;
                }
            }
            int high = R.size() - 1;
            quicksort(R, 0, high);
            quicksort(G, 0, high);
            quicksort(B, 0, high);
            dst(x, y).r = R.at(count / 2 + 1);
            dst(x, y).g = G.at(count / 2 + 1);
            dst(x, y).b = B.at(count / 2 + 1);
        }
    }
} */

void separarCanais(ppm src, pgm &r, pgm &g, pgm &b) {
    setHeader(r, "P5", src.w, src.h, 255);
    setHeader(g, "P5", src.w, src.h, 255);
    setHeader(b, "P5", src.w, src.h, 255);
    for (int y = 0; y < src.h; y++) {
        for (int x = 0; x < src.w; x++) {
            rgb pixel = src(x, y);
            r.pixel[x + src.w * y] = pixel.r;
            g.pixel[x + src.w * y] = pixel.g;
            b.pixel[x + src.w * y] = pixel.b;
        }
    }
}

void juntarCanais(ppm &dst, pgm r, pgm g, pgm b) {
    setHeader(dst, "P6", r.w, r.h, 255);
    for (int y = 0; y < r.h; y++) {
        for (int x = 0; x < r.w; x++) {
            rgb pixel;
            pixel.r = r.pixel[x + r.w * y];
            pixel.g = g.pixel[x + r.w * y];
            pixel.b = b.pixel[x + r.w * y];
            dst(x, y) = pixel;
        }
    }
}

void medianargb(ppm src, ppm &dst, int range) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(mediana, ref(r), ref(r), range);
    thread th2(mediana, ref(g), ref(g), range);
    thread th3(mediana, ref(b), ref(b), range);
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}


void laplacianorgb(ppm src, ppm &dst) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(laplaciano, ref(r), ref(r));
    thread th2(laplaciano, ref(g), ref(g));
    thread th3(laplaciano, ref(b), ref(b));
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}


void highboostrgb(ppm src, ppm &dst, int range, float k) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(highBoost, ref(r), ref(r), range, k);
    thread th2(highBoost, ref(g), ref(g), range, k);
    thread th3(highBoost, ref(b), ref(b), range, k);
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}


void ampliarrgb(ppm src, ppm &dst) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(ampliar, ref(r), ref(r));
    thread th2(ampliar, ref(g), ref(g));
    thread th3(ampliar, ref(b), ref(b));
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}


void reduzirrgb(ppm src, ppm &dst) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(reduzir, ref(r), ref(r));
    thread th2(reduzir, ref(g), ref(g));
    thread th3(reduzir, ref(b), ref(b));
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}


void normalizarrgb(ppm src, ppm &dst) {
    pgm r, g, b;
    separarCanais(src, r, g, b);
    thread th1(normalizar, ref(r), ref(r));
    thread th2(normalizar, ref(g), ref(g));
    thread th3(normalizar, ref(b), ref(b));
    th1.join();
    th2.join();
    th3.join();
    juntarCanais(dst, r, g, b);
}
