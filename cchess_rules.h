#ifndef CCHESS_RULES_H
#define CCHESS_RULES_H

#include <QObject>
#include <vector>
#include <iostream>


#include <QDebug>
#include <QImage>
//#include <QPoint>

// A chinese chess board in ascii
//      a b c d e f g h i
//    -------------------
//    9|r h e a g a e h r|9
//    8| | | | | | | | | |8
//    7| |c| | | | | |c| |7
//    6|s| |s| |s| |s| |s|6
//    5| | | | | | | | | |5
//    4| | | | | | | | | |4
//    3|s| |s| |s| |s| |s|3
//    2| |c| | | | | |c| |2
//    1| | | | | | | | | |1
//    0|r h e a g a e h r|0
//    -------------------
//      a b c d e f g h i

#define ROWS 10
#define COLS 9

enum class viewStyleMode { traditional_native, traditional_png, western_png };

enum class pieceType { Empty, General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier };

enum class markerType { Kreuz, Kreis, Dreieck, Linie, Linienende, Viereck };

enum class completePieceType {
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

enum class Color { Red, Black };

enum class PieceType {
    General,
    Advisor,
    Elephant,
    Horse,
    Chariot,
    Cannon,
    Soldier,
    Empty
};

struct PPiece {
    PieceType piece_type;
    Color color;
    QPoint pos;
    QString name;
    QImage img;
};

class  Position {

public:
  struct Square {
    PPiece *piece;
  };
  Square board[10][9];
  Color players_color;

  Position() {
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              board[rank][file].piece = nullptr;
          }
      }
  }
   void initBoard() {
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              board[rank][file].piece = nullptr;
          }
      }
      // Adding red pieces
      add_piece(new PPiece{PieceType::Chariot, Color::Red, QPoint(0, 0), "\u8ECA",
                           QImage()},
                0, 0);
      add_piece(new PPiece{PieceType::Chariot, Color::Red, QPoint(8, 0), "\u8ECA",
                           QImage()},
                8, 0);
      add_piece(new PPiece{PieceType::Horse, Color::Red, QPoint(1, 0), "\u99AC",
                           QImage()},
                1, 0);
      add_piece(new PPiece{PieceType::Horse, Color::Red, QPoint(7, 0), "\u99AC",
                           QImage()},
                7, 0);
      ;
      add_piece(new PPiece{PieceType::Elephant, Color::Red, QPoint(2, 0),
                           "\u76f8", QImage()},
                2, 0);
      ;
      add_piece(new PPiece{PieceType::Elephant, Color::Red, QPoint(6, 0),
                           "\u76f8", QImage()},
                6, 0);
      ;
      add_piece(new PPiece{PieceType::Advisor, Color::Red, QPoint(3, 0), "\u4ed5",
                           QImage()},
                3, 0);
      ;
      add_piece(new PPiece{PieceType::Advisor, Color::Red, QPoint(5, 0), "\u4ed5",
                           QImage()},
                5, 0);
      ;
      add_piece(new PPiece{PieceType::General, Color::Red, QPoint(4, 0), "\u5e25",
                           QImage()},
                4, 0);
      ;
      add_piece(new PPiece{PieceType::Cannon, Color::Red, QPoint(1, 2), "\u70ae",
                           QImage()},
                1, 2);
      ;
      add_piece(new PPiece{PieceType::Cannon, Color::Red, QPoint(7, 2), "\u70ae",
                           QImage()},
                7, 2);
      ;
      for (int i = 0; i < 9; i = i + 2) {
          add_piece(new PPiece{PieceType::Soldier, Color::Red, QPoint(i, 3),
                               "\u5175", QImage()},
                    i, 3);
          ;
      }

      // Adding black pieces
      add_piece(new PPiece{PieceType::Chariot, Color::Black, QPoint(0, 9),
                           "\u8ECA", QImage()},
                0, 9);
      add_piece(new PPiece{PieceType::Chariot, Color::Black, QPoint(8, 9),
                           "\u8ECA", QImage()},
                8, 9);
      add_piece(new PPiece{PieceType::Horse, Color::Black, QPoint(1, 9), "\u99AC",
                           QImage()},
                1, 9);
      add_piece(new PPiece{PieceType::Horse, Color::Black, QPoint(7, 9), "\u99AC",
                           QImage()},
                7, 9);
      add_piece(new PPiece{PieceType::Elephant, Color::Black, QPoint(2, 9),
                           "\u8c61", QImage()},
                2, 9);
      add_piece(new PPiece{PieceType::Elephant, Color::Black, QPoint(6, 9),
                           "\u8c61", QImage()},
                6, 9);
      add_piece(new PPiece{PieceType::Advisor, Color::Black, QPoint(3, 9),
                           "\u58eb", QImage()},
                3, 9);
      add_piece(new PPiece{PieceType::Advisor, Color::Black, QPoint(5, 9),
                           "\u58eb", QImage()},
                5, 9);
      add_piece(new PPiece{PieceType::General, Color::Black, QPoint(4, 9),
                           "\u5c07", QImage()},
                4, 9);
      add_piece(new PPiece{PieceType::Cannon, Color::Black, QPoint(1, 7),
                           "\u7832", QImage()},
                1, 7);
      add_piece(new PPiece{PieceType::Cannon, Color::Black, QPoint(7, 7),
                           "\u7832", QImage()},
                7, 7);
      for (int i = 0; i < 9; i = i + 2) {
          add_piece(new PPiece{PieceType::Soldier, Color::Black, QPoint(i, 6),
                               "\u5352", QImage()},
                    i, 6);
      }
  }

   void move_piece(int file_from, int rank_from, int file_to, int rank_to) {
      qDebug() << "move_piece...";
      qDebug() << "from (rank-file): " << rank_from << file_from;
      qDebug() << "to: " << rank_to << file_to;
      if (is_inside_board(rank_from, file_from) &&
          is_inside_board(rank_to, file_to)) {
          board[file_to][rank_to].piece = board[file_from][rank_from].piece;
          board[file_from][rank_from].piece = nullptr;
      } else
          qDebug() << "Error in move_piece";
      printBoard();
  }

   std::vector<std::pair<int, int> > generate_piece_moves(PieceType piece_type, int file, int rank) const {
      std::vector<std::pair<int, int>> moves;

      switch (piece_type) {
      case PieceType::General:
          generate_general_moves(file, rank, moves);
          break;
      case PieceType::Advisor:
          generate_advisor_moves(file, rank, moves);
          break;
      case PieceType::Elephant:
          generate_elephant_moves(file, rank, moves);
          break;
      case PieceType::Horse:
          generate_horse_moves(file, rank, moves);
          break;
      case PieceType::Chariot:
          generate_chariot_moves(file, rank, moves);
          break;
      case PieceType::Cannon:
          generate_cannon_moves(file, rank, moves);
          break;
      case PieceType::Soldier:
          generate_soldier_moves(file, rank, moves);
          break;
      default:
          break;
      }
      std::vector<std::pair<int, int>> move;

      // clean from selfhitting pieces
      for (auto m : moves) {
          if (board[m.second][m.first].piece == nullptr
              || board[m.second][m.first].piece->color != board[rank][file].piece->color
              ||  board[rank][file].piece->piece_type == PieceType::Cannon)
              move.push_back(m);
      }

      return move;
  }

   std::vector<std::pair<int, int> > generate_valid_piece_moves(PieceType piece_type, int file, int rank)  {
      std::vector<std::pair<int, int>> moves, valMoves;
      moves = generate_piece_moves(piece_type, file, rank);

      Square board_copy[10][9];

      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              board_copy[rank][file] = board[rank][file];
          }
      }

      // clean from check and ckeckmate
      for (auto move : moves)
      {
          PPiece *piece =
              board_copy[rank][file].piece;
          PPiece *captured_piece = board_copy[move.second][move.first].piece;
          Color color = piece->color;

          bool is_really_evil_glare = is_evil_glare({file,rank}, {move.first,move.second}, color);

          board_copy[move.second][move.first].piece = piece;
          board_copy[file][rank].piece = nullptr;

          // Check if the check is still present
          bool is_still_check = is_check(color);

          if (!is_still_check && !is_really_evil_glare) {
              add_move(move.first,move.second, valMoves);
          }

          // Undo the move
          board_copy[rank][file].piece = piece;;
          board_copy[move.second][move.first].piece = captured_piece;
      }

      return valMoves;
  }

   std::vector<std::pair<QPoint, QPoint> > generate_all_moves() {
      std::vector<std::pair<int, int>> moves;
      std::vector<std::pair<QPoint, QPoint>> all_moves;

      // find all possible moves
      for (int file1 = 0; file1 < 9; file1++) {
          for (int rank1 = 0; rank1 < 10; rank1++) {
              if (board[rank1][file1].piece != nullptr)
                  if (board[rank1][file1].piece->color == players_color &&
                      is_inside_board(file1, rank1)) {
                      PieceType piece_type = board[rank1][file1].piece->piece_type;
                      moves = generate_piece_moves(piece_type, file1, rank1);
                      for (auto m : moves) {
                          all_moves.push_back(std::make_pair(QPoint(file1, rank1),
                                                             QPoint(m.first, m.second)));
                      }
                  }
          }
      }
      return all_moves;
  }

  std::vector<std::pair<QPoint, QPoint> > generate_all_valid_moves() {
      // find all valid moves

      Square board_copy[10][9];

      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              board_copy[rank][file] = board[rank][file];
          }
      }

      std::vector<std::pair<QPoint, QPoint>> posVal;
      std::vector<std::pair<QPoint, QPoint>> posAll = generate_all_moves();
      // clean from check and ckeckmate
      for (auto move : posAll)
      {
          PPiece *captured_piece =
              board_copy[move.second.y()][move.second.x()].piece;
          PPiece *piece = board_copy[move.first.y()][move.first.x()].piece;
          Color color = piece->color;

          bool is_really_evil_glare = is_evil_glare(move.first, move.second, color);

          board_copy[move.second.y()][move.second.x()].piece = piece;
          board_copy[move.first.y()][move.first.x()].piece = nullptr;

          // Check if the check is still present
          bool is_still_check = is_check(color);

          if (!is_still_check && !is_really_evil_glare) {
              add_move(move.first,move.second, posVal);
          }

          // Undo the move
          board_copy[move.first.y()][move.first.x()].piece = piece;;
          board_copy[move.second.y()][move.second.x()].piece = captured_piece;
      }

      return posVal;
  }

  bool is_check(Color color) const {
      int general_file = -1;
      int general_rank = -1;

      // Find the position of the general
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              PPiece *piece = board[rank][file].piece;
              if (piece != nullptr && piece->piece_type == PieceType::General &&
                  piece->color == color) {
                  general_file = file;
                  general_rank = rank;
                  break;
              }
          }
          if (general_file != -1 && general_rank != -1) {
              break;
          }
      }

      // Check if the opponent's pieces can attack the general
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              PPiece *piece = board[rank][file].piece;
              if (piece != nullptr && piece->color != color) {
                  std::vector<std::pair<int, int>> moves =
                      generate_piece_moves(piece->piece_type, file, rank);
                  int i = 0;
                  for (const auto &move : moves) {
                      if (move.first == general_file && move.second == general_rank) {
                          return true;
                      }
                  }
              }
          }
      }

      return false;
  }

  bool is_checkmate(Color color, std::vector<std::pair<int, int> > &moves) {
      // Check if the general is in check
      if (!is_check(color)) {
          return false;
      }

      // Try moving each of the player's pieces to see if the check can be escaped
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              PPiece *piece = board[rank][file].piece;
              if (piece != nullptr && piece->color == color) {
                  std::vector<std::pair<int, int>> moves =
                      generate_piece_moves(piece->piece_type, file, rank);
                  for (const auto &move : moves) {
                      // Try the move
                      PPiece *captured_piece =
                          board[move.second][move.first].piece;
                      board[move.second][move.first].piece = piece;
                      board[rank][file].piece = nullptr;

                      // Check if the check is still present
                      bool is_still_check = is_check(color);

                      // Undo the move
                      board[rank][file].piece = piece;
                      board[move.second][move.first].piece = captured_piece;

                      if (!is_still_check) {
                          // The check can be escaped
                          return false;
                      }
                  }
              }
          }
      }

      // All possible moves have been checked and the general is still in check
      // Checkmate!
      return true;
  }

 bool is_evil_glare(QPoint from, QPoint to, Color color) const {
      // std::cout << "info is_evil_glase()" << std::endl;

      Square board_copy[10][9];

      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              board_copy[rank][file] = board[rank][file];
          }
      }

      board_copy[to.y()][to.x()] = board_copy[from.y()][from.x()];
      board_copy[from.y()][from.x()].piece = nullptr;

      int general_file = -1;
      int general_rank = -1;
      int opp_general_file = -1;
      int opp_general_rank = -1;
      //PPiece *general = nullptr;

      // Find the position of the general
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              PPiece *piece = board_copy[rank][file].piece;
              if (piece != nullptr && piece->piece_type == PieceType::General &&
                  piece->color == color) {
                  general_file = file;
                  general_rank = rank;
                  break;
              }
          }
          if (general_file != -1 && general_rank != -1) {
              break;
          }
      }
      //if (general_file != -1 && general_rank != -1) {
      //    general = board_copy[general_rank][general_file].piece;
      //} else
      //    std::cout << "info error in is_evil_glare" << std::endl;

      // Find the position of the opposite general
      for (int rank = 0; rank < 10; ++rank) {
          for (int file = 0; file < 9; ++file) {
              PPiece *piece = board_copy[rank][file].piece;
              if (piece != nullptr && piece->piece_type == PieceType::General &&
                  piece->color != color) {
                  opp_general_file = file;
                  opp_general_rank = rank;
                  break;
              }
          }
          if (opp_general_file != -1 && opp_general_rank != -1) {
              break;
          }
      }

      if (opp_general_file == general_file) {
          if (opp_general_rank < general_rank) {
              for (size_t i = opp_general_rank + 1; i < general_rank; i++) {
                  if (board_copy[i][general_file].piece != nullptr) {
                      return false;
                  }
              }
          } else {
              for (size_t i = general_rank + 1; i < opp_general_rank; i++) {
                  if (board_copy[i][opp_general_file].piece != nullptr) {
                      return false;
                  }
              }
          }
      } else
          return false;
      return true;
  }

  void generate_general_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
      for (const auto &dir : directions) {
          PPiece *piece = board[rank][file].piece;
          if ((rank + dir[1] >= 0 && rank + dir[1] <= 2) && file + dir[0] >= 3 &&
              file + dir[0] <= 5 && piece->color == Color::Red)
              add_move(file + dir[0], rank + dir[1], moves);
          if ((rank + dir[1] >= 7 && rank + dir[1] <= 9) && file + dir[0] >= 3 &&
              file + dir[0] <= 5 && piece->color == Color::Black)
              add_move(file + dir[0], rank + dir[1], moves);
      }
  }

  void generate_advisor_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[4][2] = {{-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
      for (const auto &dir : directions) {
          PPiece *piece = board[rank][file].piece;
          if ((rank + dir[1] >= 0 && rank + dir[1] <= 2) && file + dir[0] >= 3 &&
              file + dir[0] <= 5 && piece->color == Color::Red)
              add_move(file + dir[0], rank + dir[1], moves);
          if ((rank + dir[1] >= 7 && rank + dir[1] <= 9) && file + dir[0] >= 3 &&
              file + dir[0] <= 5 && piece->color == Color::Black)
              add_move(file + dir[0], rank + dir[1], moves);
      }
  }

  void generate_elephant_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[4][2] = {{-2, -2}, {2, 2}, {2, -2}, {-2, 2}};
      int between_directions[4][2] = {{-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
      for (int i = 0; i < 4; ++i) {
          int new_file = file + directions[i][0];
          int new_rank = rank + directions[i][1];
          int between_file = file + between_directions[i][0];
          int between_rank = rank + between_directions[i][1];
          if (is_inside_board(new_file, new_rank) &&
              board[new_rank][new_file].piece == nullptr &&
              is_inside_board(between_file, between_rank) &&
              board[between_rank][between_file].piece == nullptr) {
              add_move(new_file, new_rank, moves);
          }
      }
  }

  void generate_horse_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
                              {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};
      for (const auto &dir : directions) {
          int new_file = file + dir[0];
          int new_rank = rank + dir[1];
          int between_file = file + dir[0] / 2;
          int between_rank = rank + dir[1] / 2;
          if (is_inside_board(new_file, new_rank) &&
              (board[new_rank][new_file].piece == nullptr ||
               board[new_rank][new_file].piece->color !=
                   board[rank][file].piece->color)) {
              if (dir[0] == -2 || dir[0] == 2) {
                  if (is_inside_board(between_file, rank) &&
                      board[rank][between_file].piece == nullptr) {
                      add_move(new_file, new_rank, moves);
                  }
              } else {
                  if (is_inside_board(file, between_rank) &&
                      board[between_rank][file].piece == nullptr) {
                      add_move(new_file, new_rank, moves);
                  }
              }
          }
      }
  }

  void generate_chariot_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
      for (const auto &dir : directions) {
          int new_file = file + dir[1];
          int new_rank = rank + dir[0];
          while (is_inside_board(new_rank, new_file)) {
              PPiece *piece = board[new_rank][new_file].piece;
              if (piece == nullptr) {
                  add_move(new_file, new_rank, moves);
              } else {
                  if (piece->color != board[rank][file].piece->color) {
                      add_move(new_file, new_rank, moves);
                  }
                  break;
              }
              new_file += dir[1];
              new_rank += dir[0];
          }
      }
  }

  void generate_cannon_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
      for (const auto &dir : directions) {
          int new_file = file + dir[0];
          int new_rank = rank + dir[1];
          bool found_piece = false;
          while (is_inside_board(new_file, new_rank)) {
              PPiece *piece = board[new_rank][new_file].piece;
              if (piece == nullptr) {
                  if (!found_piece) {
                      add_move(new_file, new_rank, moves);
                  }
              } else {
                  if (!found_piece) {
                      found_piece = true;
                  } else {
                      if (piece->color != board[rank][file].piece->color) {
                          add_move(new_file, new_rank, moves);
                      }
                      break;
                  }
              }
              new_file += dir[0];
              new_rank += dir[1];
          }
      }
  }

  void generate_soldier_moves(int file, int rank, std::vector<std::pair<int, int> > &moves) const {
      int directions[3][2];
      if (board[rank][file].piece->color == Color::Red) {
          directions[0][0] = 0;
          directions[0][1] = 1;
          if (rank > 4) {
              directions[1][0] = -1;
              directions[1][1] = 0;
              directions[2][0] = 1;
              directions[2][1] = 0;
          }
      } else {
          directions[0][0] = 0;
          directions[0][1] = -1;

          if (rank < 4) {
              directions[1][0] = 1;
              directions[1][1] = 0;
              directions[2][0] = -1;
              directions[2][1] = 0;
          }
      }

      for (const auto &dir : directions) {
          int new_file = file + dir[0];
          int new_rank = rank + dir[1];
          if (is_inside_board(new_file, new_rank)) {
              PPiece *piece = board[new_rank][new_file].piece;
              // if (new_file == 4 && new_rank == 6)
              //     std::cout << "test" << std::endl;

              if (piece == nullptr ||
                  piece->color != board[rank][file].piece->color) {

                  add_move(new_file, new_rank, moves);
              }
          }
      }
  }

  bool is_inside_board(int file, int rank) const {
    return file >= 0 && file < 9 && rank >= 0 && rank < 10;
  }

  void add_piece(PPiece *piece, int file, int rank) {
    if (is_inside_board(file, rank)) {
      board[rank][file].piece = piece;
    }
  }


  // .:Helper functions:.
  // Gibt das Brett in der Konsole aus
  void printBoard() {
      for (int i = 0; i < 10; i++) {
          QString line = "";
          for (int j = 0; j < 9; j++) {
              if (board[i][j].piece == nullptr)
                  line += "0";
              else
                  line += QString::number((int)board[i][j].piece->piece_type + 1);
              line += " ";
          }
          qDebug() << line;
      }
  };

  void toggleColor(Color *c) {
    if (*c == Color::Red)
      *c = Color::Black;
    else
      *c = Color::Red;
  }




  void add_move(QPoint from, QPoint to,
                std::vector<std::pair<QPoint, QPoint>> &moves) const {
      if (is_inside_board(to.y(), to.x())) {
          moves.push_back(std::pair(from, to));
      }
  }

  void add_move(int file, int rank,
                std::vector<std::pair<int, int>> &moves) const {
    if (is_inside_board(file, rank)) {
        moves.push_back(std::make_pair(file, rank));
      }
  }

};

#endif // CCHESS_RULES_H
