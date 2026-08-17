#pragma once

#include "tetra/Tetra.h"

#include <thread>
#include <vector>
#include <optional>

#include <extern/synchronized.h>

class CursesTetraBoardRenderer {
    const size_t mMaxX;
    const size_t mMaxY;

    bool mHardPush{};
    sv::synchronized<std::vector<TetraInput>> mInputSync{};
    sv::synchronized<std::vector<TetraBlock>> mBoardSync{};

    bool mIsRunning{true};
    std::thread mProcessingThread;

    [[nodiscard]] std::optional<TetraInput> translateInput(int c) const;
    void privateRender(const std::vector<TetraBlock> &currentBoard) const;

public:
    explicit CursesTetraBoardRenderer(const TetraWorldInterface &world);
    ~CursesTetraBoardRenderer();

    void stopThread();
    void render(std::vector<TetraBlock> currentBoard);
    std::vector<TetraInput> getInputs();
    bool hardPushPressed();

    [[noreturn]] void startThread();
};
