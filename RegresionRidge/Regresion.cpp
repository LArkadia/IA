#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Normalization using Z-score
std::vector<double> zScoreNormalization(const std::vector<double>& data) {
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double stddev = std::sqrt(std::accumulate(data.begin(), data.end(), 0.0, 
                      [mean](double acc, double x) { return acc + (x - mean) * (x - mean); }) / data.size());

    std::vector<double> normalized(data.size());
    std::transform(data.begin(), data.end(), normalized.begin(), [mean, stddev](double x) {
        return (x - mean) / stddev;
    });
    return normalized;
}

// Ridge regression parameters
struct RidgeParams {
    std::vector<double> B;
    double learningRate;
    double lambda1;
    double lambda2;
};

// Compute cost
double computeCost(const std::vector<double>& yd, const std::vector<double>& Yobt, const std::vector<double>& B, double lambda2, int m) {
    double error = 0.0;
    for (size_t i = 0; i < yd.size(); ++i) {
        error += (yd[i] - Yobt[i]) * (yd[i] - Yobt[i]);
    }
    error = error / (2 * m);
    double L2 = (lambda2 / 2) * std::inner_product(B.begin(), B.end(), B.begin(), 0.0);
    return error + L2;
}

void drawGraph(SDL_Renderer* renderer, const std::vector<double>& points) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (size_t i = 1; i < points.size(); ++i) {
        SDL_RenderDrawLine(renderer,
                           (i - 1) * WINDOW_WIDTH / points.size(), 
                           WINDOW_HEIGHT - static_cast<int>(points[i - 1] * 100),
                           i * WINDOW_WIDTH / points.size(), 
                           WINDOW_HEIGHT - static_cast<int>(points[i] * 100));
    }
    SDL_RenderPresent(renderer);
}

int main() {
    // Data
    std::vector<double> X1 = {1, 3, 6, 9, 11, 15, 16, 19, 24};
    std::vector<double> X2 = {100, 200, 300, 400, 500, 600, 700, 800, 900};
    std::vector<double> yd = {4, 5, 6.5, 7, 8.5, 12, 13, 16, 22};
    int m = X1.size();

    // Normalize data
    X1 = zScoreNormalization(X1);
    X2 = zScoreNormalization(X2);

    // Initialize parameters
    RidgeParams params = {{static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX}, 0.1, 0.1, 0.001};
    int maxEpochs = 10000;
    double bestError = std::numeric_limits<double>::infinity();
    double errorDiff = 0.0001;
    int patience = 10;
    int patienceCounter = 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ridge Regression", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Training loop
    std::vector<double> costs;
    for (int epoch = 0; epoch < maxEpochs; ++epoch) {
        // Compute predictions
        std::vector<double> Yobt(m);
        for (int i = 0; i < m; ++i) {
            Yobt[i] = params.B[0] + params.B[1] * X1[i] + params.B[2] * X2[i];
        }

        // Compute cost
        double cost = computeCost(yd, Yobt, params.B, params.lambda2, m);
        costs.push_back(cost);

        // Early stopping
        if (bestError - cost < errorDiff) {
            patienceCounter++;
            if (patienceCounter >= patience) {
                std::cout << "Early stop at epoch: " << epoch << std::endl;
                break;
            }
        } else {
            patienceCounter = 0;
        }
        bestError = std::min(bestError, cost);

        // Gradient descent
        std::vector<double> gradients(3, 0.0);
        for (int i = 0; i < m; ++i) {
            double error = Yobt[i] - yd[i];
            gradients[0] += error;
            gradients[1] += error * X1[i];
            gradients[2] += error * X2[i];
        }

        params.B[0] -= params.learningRate / m * gradients[0];
        params.B[1] -= params.learningRate / m * gradients[1] + params.lambda1 / m * params.B[1];
        params.B[2] -= params.learningRate / m * gradients[2] + params.lambda1 / m * params.B[2];

        // Draw graph
        drawGraph(renderer, costs);
        SDL_Delay(50);
    }

    std::cout << "Final parameters: B0 = " << params.B[0] << ", B1 = " << params.B[1] << ", B2 = " << params.B[2] << std::endl;

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
