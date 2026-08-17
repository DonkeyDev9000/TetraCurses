#pragma once

#include "tetris/Tetra.h"

#include <thread>
#include <vector>
#include <optional>

#include <extern/synchronized.h>

class CursesTetraBoardRenderer {
    const int mMaxX;
    const int mMaxY;

    sv::synchronized<std::vector<TetraInput>> mInputSync{};
    sv::synchronized<std::vector<TetraBlock>> mBoardSync{};

    std::thread mProcessingThread;

    [[nodiscard]] std::optional<TetraInput> translateInput(int c) const;
    void privateRender(const std::vector<TetraBlock> &currentBoard) const;

public:
    CursesTetraBoardRenderer(int maxX, int maxY);
    ~CursesTetraBoardRenderer();

    void render(std::vector<TetraBlock> currentBoard);
    std::vector<TetraInput> getInputs();

    [[noreturn]] void startThread();
};
