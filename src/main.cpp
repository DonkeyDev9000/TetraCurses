#include <iostream>

#include "tetris/Tetra.h"
#include "tetris/TetraWorldImpl.h"
#include "curses/TetraCurses.h"

int main() {
    static constexpr std::chrono::milliseconds updatesPerSecond{500};
    static constexpr int sMaxX{10};
    static constexpr int sMaxY{18};

    const auto board = std::make_shared<TetraBoard>(sMaxX, sMaxY);

    std::shared_ptr<CursesTetraBoardRenderer> rederer{};
    rederer = std::make_shared<CursesTetraBoardRenderer>(sMaxX, sMaxY);

    board->initialize();

    auto lastStep = std::chrono::system_clock::now();
    bool inputHandled{true};
    do {
        const auto now = std::chrono::system_clock::now();

        bool hardPush{false};
        if (rederer) {
            const auto inputsVec = rederer->getInputs();
            // TODO We need to split up the inputs for the hardpush
            if (!inputsVec.empty()) {
                inputHandled = true;
                board->inputReceived(inputsVec);
            }
        }

        if (hardPush || (now - lastStep) >= updatesPerSecond) {
            lastStep = now;
            board->worldStep(WorldStepParams{hardPush});
            inputHandled = true;
        }

        if (rederer && inputHandled) {
            rederer->render(board->getCurrentWorld());
        }
        inputHandled = false;
    } while (true);

    return 0;
}