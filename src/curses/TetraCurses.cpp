#include "TetraCurses.h"

#include "ncurses.h"

CursesTetraBoardRenderer::CursesTetraBoardRenderer(const TetraWorldInterface &world) :
    mMaxX(world.getWidth()),
    mMaxY(world.getHeight()) {
    mProcessingThread = std::thread{&CursesTetraBoardRenderer::startThread, this};
}
CursesTetraBoardRenderer::~CursesTetraBoardRenderer() {
    mProcessingThread.join();
}
void CursesTetraBoardRenderer::stopThread() {
    mIsRunning = false;
}
void CursesTetraBoardRenderer::startThread() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    do {
        if (const auto input = getch();
            input != ERR) {
            if (auto translatedOpt = translateInput(input)) {
                auto wLock = mInputSync.wlock();
                wLock->emplace_back(*translatedOpt);
            }
            if (input == KEY_UP) {
                mHardPush = true;
            }
        }

        std::vector<TetraBlock> toRender{};

        {
            auto w = mBoardSync.wlock();
            w->swap(toRender);
        }
        if (!toRender.empty()) {
            privateRender(toRender);
        }

    } while (mIsRunning);
    // todo make sure we have enough screen room to print our board.
}
void CursesTetraBoardRenderer::render(std::vector<TetraBlock> currentBoard) {
    auto w = mBoardSync.wlock();
    w->swap(currentBoard);
}
std::vector<TetraInput> CursesTetraBoardRenderer::getInputs() {
    std::vector<TetraInput> retVal{};
    {
        auto l = mInputSync.wlock();
        l->swap(retVal);
    }

    return retVal;
}
bool CursesTetraBoardRenderer::hardPushPressed() {
    auto retVal = mHardPush;
    mHardPush = false;
    return retVal;
}
std::optional<TetraInput> CursesTetraBoardRenderer::translateInput(int ch) const {
    switch (ch) {
        case KEY_LEFT:
            return moveLeft;
        case KEY_RIGHT:
            return moveRight;
        case KEY_DOWN:
            return moveDown;
        case int{'q'}:
        case int{'Q'}:
            return spinLeft;
        case int{'e'}:
        case int{'E'}:
            return spinRight;

        default:
            return std::nullopt;
    }
}
void CursesTetraBoardRenderer::privateRender(const std::vector<TetraBlock> &currentBoard) const {
    for (int x = 0; x < mMaxX; x++) {
        for (int y = 0; y < mMaxY; y++) {
            mvprintw(y, x, "_");
        }
    }

    for (const auto &[x, y] : currentBoard) {
        if (x >= 0 && x < mMaxX && y >= 0 && y < mMaxY) {
            mvprintw(y, x, "X");
        }
    }
    refresh();
}