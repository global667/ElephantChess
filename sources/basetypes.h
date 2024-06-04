#ifndef BASETYPES_H
#define BASETYPES_H

enum struct PieceType {
    General,
    Advisor,
    Elephant,
    Horse,
    Chariot,
    Cannon,
    Soldier,
    Empty
};

enum struct Color { Red, Black, None };

struct Point {
    int x, y;
    Point(const int x = 0, const int y = 0) : x(x), y(y) {}
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point &other) const {
        return x != other.x || y != other.y;
    }

    bool operator<(const Point &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    bool operator>(const Point &other) const {
        return x > other.x || (x == other.x && y > other.y);
    }

    bool operator<=(const Point &other) const {
        return x <= other.x || (x == other.x && y <= other.y);
    }

    bool operator>=(const Point &other) const {
        return x >= other.x || (x == other.x && y >= other.y);
    }

    Point operator+(const Point &other) const {
        return {x + other.x, y + other.y};
    }

    Point operator-(const Point &other) const {
        return {x - other.x, y - other.y};
    }

    bool operator*(const Point &other) const {
        return x == other.x && y == other.y;
    }

    [[nodiscard]] bool isValid() const {
        return x >= 0 && x < 9 && y >= 0 && y < 10;
    }
};

enum struct markerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };

static constexpr int ROWS = 10;
static constexpr int COLS = 9;

enum struct completePieceType {
    GeneralRot,
    AdvisorRot,
    ElephantRot,
    HorseRot,
    ChariotRot,
    CannonRot,
    SoldierRot,
    GeneralSchwarz,
    AdvisorSchwarz,
    ElephantSchwarz,
    HorseSchwarz,
    ChariotSchwarz,
    CannonSchwarz,
    SoldierSchwarz
};

#endif // BASETYPES_H
