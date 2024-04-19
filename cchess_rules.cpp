
#include "cchess_rules.h"

//extern BaseModel basemodel;

void Position::initBoard() {
	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			add_piece(new PPiece{ PieceType::Empty,Color::Red,"",QImage() }, rank, file);
		}
	}
	// Adding red pieces
	add_piece(new PPiece{ PieceType::Chariot, Color::Red, "\u8ECA",	QImage() }, 0, 0);
	add_piece(new PPiece{ PieceType::Chariot, Color::Red,  "\u8ECA", QImage() }, 8, 0);
	add_piece(new PPiece{ PieceType::Horse, Color::Red, "\u99AC", QImage() }, 1, 0);
	add_piece(new PPiece{ PieceType::Horse, Color::Red,  "\u99AC", QImage() }, 7, 0);
	add_piece(new PPiece{ PieceType::Elephant, Color::Red, "\u76f8", QImage() }, 2, 0);
	add_piece(new PPiece{ PieceType::Elephant, Color::Red, 	"\u76f8", QImage() }, 6, 0);
	add_piece(new PPiece{ PieceType::Advisor, Color::Red, "\u4ed5",	QImage() }, 3, 0);
	add_piece(new PPiece{ PieceType::Advisor, Color::Red, "\u4ed5",	QImage() }, 5, 0);
	add_piece(new PPiece{ PieceType::General, Color::Red, "\u5e25",	QImage() }, 4, 0);
	add_piece(new PPiece{ PieceType::Cannon, Color::Red, "\u70ae", QImage() }, 1, 2);
	add_piece(new PPiece{ PieceType::Cannon, Color::Red, "\u70ae", QImage() }, 7, 2);
	for (int i = 0; i < 9; i = i + 2) {
		add_piece(new PPiece{ PieceType::Soldier, Color::Red, "\u5175", QImage() }, i, 3);
	}

	// Adding black pieces
	add_piece(new PPiece{ PieceType::Chariot, Color::Black, "\u8ECA", QImage() }, 0, 9);
	add_piece(new PPiece{ PieceType::Chariot, Color::Black, "\u8ECA", QImage() }, 8, 9);
	add_piece(new PPiece{ PieceType::Horse, Color::Black, "\u99AC", QImage() }, 1, 9);
	add_piece(new PPiece{ PieceType::Horse, Color::Black, "\u99AC",	QImage() }, 7, 9);
	add_piece(new PPiece{ PieceType::Elephant, Color::Black, "\u8c61", QImage() }, 2, 9);
	add_piece(new PPiece{ PieceType::Elephant, Color::Black, "\u8c61", QImage() }, 6, 9);
	add_piece(new PPiece{ PieceType::Advisor, Color::Black, "\u58eb", QImage() }, 3, 9);
	add_piece(new PPiece{ PieceType::Advisor, Color::Black, "\u58eb", QImage() }, 5, 9);
	add_piece(new PPiece{ PieceType::General, Color::Black, "\u5c07", QImage() }, 4, 9);
	add_piece(new PPiece{ PieceType::Cannon, Color::Black, "\u7832", QImage() }, 1, 7);
	add_piece(new PPiece{ PieceType::Cannon, Color::Black, "\u7832", QImage() }, 7, 7);
	for (int i = 0; i < 9; i = i + 2) {
		add_piece(new PPiece{ PieceType::Soldier, Color::Black,	"\u5352", QImage() }, i, 6);
	}
}

void Position::move_piece(int file_from, int rank_from, int file_to, int rank_to) 
{
	if (is_inside_board(rank_from, file_from) &&
		is_inside_board(rank_to, file_to)) {
		*board[file_to][rank_to].piece = *board[file_from][rank_from].piece;

		board[file_from][rank_from].piece->piece_type = PieceType::Empty;
		board[file_from][rank_from].piece->color = Color::Red;
		board[file_from][rank_from].piece->name = "";
	}
	else
		qDebug() << "Error in move_piece";
}

// Generates the moves vector of one piece
std::vector<std::pair<int, int>> Position::generate_piece_moves_for(PieceType piece_type, int file, int rank) const {
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
	return moves;
}

// Generates the real valid moves for one piece (called from Boardview)
std::vector<std::pair<int, int>> Position::generate_valid_moves_for(PieceType piece_type, int rank, int file) 
{
	std::vector<std::pair<int, int>> moves, valMoves;

	// TODO: need to correcting rank and file !!!!
	int tmp = rank;
	rank = file;
	file = tmp;
	moves = generate_piece_moves_for(piece_type, rank, file);

	PPiece board_copy[10][9];

	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			board_copy[rank][file] = *board[rank][file].piece;
		}
	}

	// clean from check and evil glare
	for (auto& move : moves)
	{
		PPiece piece =
			board_copy[rank][file];
		PPiece captured_piece = board_copy[move.first][move.second];
		Color color = piece.color;

		auto to = QPoint(move.first, move.second);
		auto from = QPoint(rank, file);
		auto moveAsPair = std::make_pair(from, to);

		make_move(moveAsPair, board_copy);

		// Check if the check is still present
		bool is_still_check = is_check(color);
		bool is_really_evil_glare = is_evil_glare(color); 

		if (!is_still_check && !is_really_evil_glare) {
			add_move(move.first, move.second, valMoves);
		}

		take_back(moveAsPair, board_copy, captured_piece, piece);
	}

	return valMoves;
}

// Generates all moves incl. chess, glare and mates
std::vector<std::pair<QPoint, QPoint>> Position::generate_piece_moves_for_all() {
	std::vector<std::pair<int, int>> moves;
	std::vector<std::pair<QPoint, QPoint>> all_moves;

	// find all possible moves
	for (int file1 = 0; file1 < 9; file1++) {
		for (int rank1 = 0; rank1 < 10; rank1++) {
			if (board[rank1][file1].piece->piece_type != PieceType::Empty)
				if (board[rank1][file1].piece->color == players_color &&
					is_inside_board(file1, rank1)) {
					PieceType piece_type = board[rank1][file1].piece->piece_type;
					moves = generate_piece_moves_for(piece_type, file1, rank1);
					for (auto& m : moves) {
						all_moves.push_back(std::make_pair(QPoint(rank1, file1),
							QPoint(m.first, m.second)));
					}
				}
		}
	}
	return all_moves;
}

// Generates the real valid moves, cleaned from all invalid moves like chess etc.
// (called from the intern engine)
std::vector<std::pair<QPoint, QPoint>> Position::generate_valid_moves_for_all() {
	// find all valid moves

	PPiece board_copy[10][9];

	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			board_copy[rank][file] = *board[rank][file].piece;
		}
	}

	std::vector<std::pair<QPoint, QPoint>> posVal;
	std::vector<std::pair<QPoint, QPoint>> posAll = generate_piece_moves_for_all();
	// clean from check and ckeckmate
	for (auto& move : posAll)
	{
		PPiece captured_piece =
			board_copy[move.second.y()][move.second.x()];
		PPiece piece = board_copy[move.first.y()][move.first.x()];
		Color color = piece.color;

		auto to = QPoint(move.first.x(), move.first.y());
		auto from = QPoint(move.second.x(), move.second.y());
		auto moveAsPair = std::make_pair(from, to);

		make_move(moveAsPair, board_copy);

		// Check if the check is still present
		bool is_still_check = is_check(color);
		bool is_really_evil_glare = is_evil_glare(color);

		if (!is_still_check && !is_really_evil_glare) {
			add_move(move.first, move.second, posVal);
		}

		take_back(moveAsPair, board_copy, captured_piece, piece);
	}

	return posVal;
}

// perft test
QString Position::perftTest(int depth) {
	nodes = 0;
	QString resultString = QString();
	resultString = "   Performance test : \n";
	QTime startTime = QTime::currentTime();

	std::vector<std::pair<QPoint, QPoint>> moveList = generate_valid_moves_for_all();

	for (int count = 0; count < moveList.size(); count++) {

		PPiece board_copy[10][9];

		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				board_copy[rank][file] = *board[rank][file].piece;
			}
		}
		PPiece captured_piece = board_copy[moveList[count].first.x()][moveList[count].first.y()];
		PPiece piece = board_copy[moveList[count].second.x()][moveList[count].second.y()];

		make_move(moveList[count], board_copy);
		int cumNodes = nodes;
		perftDriver(depth - 1);
		take_back(moveList[count], board_copy, captured_piece, piece);
	}
	resultString += QString("\n   Depth : %1").arg(depth);
	resultString += QString("\n   Nodes : %1").arg(nodes);
	resultString += QString("\n  Checks : %1").arg(checks);
	resultString += QString("\nCaptures : %1").arg(captures);
	resultString += QString("\n    Time : %1 ms\n").arg(-QTime::currentTime().msecsTo(startTime));

	return resultString;
}

void Position::perftDriver(int depth) {
	if (depth == 0) { nodes++; return; }

	std::vector<std::pair<QPoint, QPoint>> moveList = generate_valid_moves_for_all();

	for (int count = 0; count < moveList.size(); count++) {
		std::pair<QPoint, QPoint> move = moveList[count];

		PPiece board_copy[10][9];

		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				board_copy[rank][file] = *board[rank][file].piece;
			}
		}
		PPiece captured_piece = board_copy[move.first.x()][move.first.y()];
		PPiece piece = board_copy[move.second.x()][move.second.y()];

		make_move(move, board_copy);
		perftDriver(depth - 1);
		take_back(move, board_copy, captured_piece, piece);
	}
}

void Position::make_move(std::pair<QPoint, QPoint> move, PPiece cboard[10][9])
{
	//captured_piece = cboard[move.second.y()][move.second.x()];
	PPiece piece = cboard[move.first.y()][move.first.x()];
	Color color = piece.color;

	if (piece.piece_type != PieceType::Empty)
		captures++;

	cboard[move.second.y()][move.second.x()] = piece;
	cboard[move.first.y()][move.first.x()].piece_type = PieceType::Empty;
	cboard[move.first.y()][move.first.x()].color = Color::Red;
	cboard[move.first.y()][move.first.x()].name = "";
}

void Position::take_back(std::pair<QPoint, QPoint> move, PPiece cboard[10][9], PPiece captured_piece, PPiece piece) const
{
	// Undo the move
	cboard[move.second.y()][move.second.x()] = piece;;
	cboard[move.first.y()][move.first.x()] = captured_piece;
}

bool Position::is_check(Color color) const {
	int general_file = -1;
	int general_rank = -1;

	// Find the position of the general
	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			PPiece* piece = board[rank][file].piece;
			if (piece->piece_type == PieceType::General &&
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
			PPiece* piece = board[rank][file].piece;
			if (piece->piece_type != PieceType::Empty && piece->color != color) {
				std::vector<std::pair<int, int>> moves =
					generate_piece_moves_for(piece->piece_type, file, rank);
				for (const auto& move : moves) {
					if (move.second == general_file && move.first == general_rank && piece->piece_type != PieceType::Empty && piece->color != color) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

inline bool Position::is_evil_glare(Color color) const 
{
	int general_file = -1;
	int general_rank = -1;
	int opp_general_file = -1;
	int opp_general_rank = -1;

	// Find the position of the general
	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			PPiece *piece = board[rank][file].piece;
			if (piece->piece_type != PieceType::Empty && piece->piece_type == PieceType::General &&
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
	// Find the position of the opposite general
	for (int rank = 0; rank < 10; ++rank) {
		for (int file = 0; file < 9; ++file) {
			PPiece *piece = board[rank][file].piece;
			if (piece->piece_type != PieceType::Empty && piece->piece_type == PieceType::General &&
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
				if (board[i][general_file].piece->piece_type != PieceType::Empty) {
					return false;
				}
			}
		}
		else {
			for (size_t i = general_rank + 1; i < opp_general_rank; i++) {
				if (board[i][opp_general_file].piece->piece_type != PieceType::Empty) {
					return false;
				}
			}
		}
	}
	else
		return false;
	return true;
}

void Position::generate_general_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	for (const auto& dir : directions)
	{

		PPiece* piece = board[rank][file].piece;

		if ((rank + dir[1] >= 0 && rank + dir[1] <= 2) && file + dir[0] >= 3 &&
			file + dir[0] <= 5)
		{
			PPiece* target = board[rank + dir[1]][file + dir[0]].piece;
			if (target->color != Color::Red || target->piece_type == PieceType::Empty)
				add_move(file + dir[0], rank + dir[1], moves);
		}

		if ((rank + dir[1] >= 7 && rank + dir[1] <= 9) && file + dir[0] >= 3 &&
			file + dir[0] <= 5 && piece->color != Color::Black)
		{
			PPiece* target = board[rank + dir[1]][file + dir[0]].piece;
			if (target->color != Color::Red || target->piece_type == PieceType::Empty)
				add_move(file + dir[0], rank + dir[1], moves);
		}
	}
}

void Position::generate_advisor_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[4][2] = { {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };
	for (const auto& dir : directions) {
		PPiece* piece = board[rank][file].piece;
		if ((rank + dir[1] >= 0 && rank + dir[1] <= 2) && file + dir[0] >= 3 &&
			file + dir[0] <= 5 && piece->color == Color::Red)
		{
			PPiece* target = board[rank + dir[1]][file + dir[0]].piece;
			if (target->color != Color::Red || target->piece_type == PieceType::Empty)
				add_move(file + dir[0], rank + dir[1], moves);
		}
		if ((rank + dir[1] >= 7 && rank + dir[1] <= 9) && file + dir[0] >= 3 &&
			file + dir[0] <= 5 && piece->color == Color::Black)
		{
			PPiece* target = board[rank + dir[1]][file + dir[0]].piece;
			if (target->color != Color::Black || target->piece_type == PieceType::Empty)
				add_move(file + dir[0], rank + dir[1], moves);
		}
	}
}

void Position::generate_elephant_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[4][2] = { {-2, -2}, {2, 2}, {2, -2}, {-2, 2} };
	int between_directions[4][2] = { {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };
	for (int i = 0; i < 4; ++i) {
		int new_file = file + directions[i][0];
		int new_rank = rank + directions[i][1];
		int between_file = file + between_directions[i][0];
		int between_rank = rank + between_directions[i][1];
		if (is_inside_board(new_file, new_rank) &&
			board[new_rank][new_file].piece->piece_type == PieceType::Empty &&
			is_inside_board(between_file, between_rank) &&
			board[between_rank][between_file].piece->piece_type == PieceType::Empty) {
			add_move(new_file, new_rank, moves);
		}
	}
}

void Position::generate_horse_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
							{-1, -2}, {-1, 2}, {1, -2}, {1, 2} };
	for (const auto& dir : directions) {
		int new_file = file + dir[0];
		int new_rank = rank + dir[1];
		int between_file = file + dir[0] / 2;
		int between_rank = rank + dir[1] / 2;
		if (is_inside_board(new_file, new_rank) &&
			(board[new_rank][new_file].piece->piece_type == PieceType::Empty ||
				board[new_rank][new_file].piece->color !=
				board[rank][file].piece->color)) {
			if (dir[0] == -2 || dir[0] == 2) {
				if (is_inside_board(between_file, rank) &&
					board[rank][between_file].piece->piece_type == PieceType::Empty) {
					add_move(new_file, new_rank, moves);
				}
			}
			else {
				if (is_inside_board(file, between_rank) &&
					board[between_rank][file].piece->piece_type == PieceType::Empty) {
					add_move(new_file, new_rank, moves);
				}
			}
		}
	}
}

void Position::generate_chariot_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	for (const auto& dir : directions) {
		int new_file = file + dir[1];
		int new_rank = rank + dir[0];
		while (is_inside_board(new_rank, new_file)) {
			PPiece* piece = board[new_rank][new_file].piece;
			if (piece->piece_type == PieceType::Empty) {
				add_move(new_file, new_rank, moves);
			}
			else {
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

void Position::generate_cannon_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
	int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	for (const auto& dir : directions) {
		int new_file = file + dir[0];
		int new_rank = rank + dir[1];
		bool found_piece = false;
		while (is_inside_board(new_file, new_rank)) {
			PPiece* piece = board[new_rank][new_file].piece;
			if (piece->piece_type == PieceType::Empty) {
				if (!found_piece) {
					add_move(new_file, new_rank, moves);
				}
			}
			else {
				if (!found_piece) {
					found_piece = true;
				}
				else {
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

void Position::generate_soldier_moves(int file, int rank, std::vector<std::pair<int, int> >& moves) const {
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
	}
	else {
		directions[0][0] = 0;
		directions[0][1] = -1;

		if (rank < 4) {
			directions[1][0] = 1;
			directions[1][1] = 0;
			directions[2][0] = -1;
			directions[2][1] = 0;
		}
	}

	for (const auto& dir : directions) {
		int new_file = file + dir[0];
		int new_rank = rank + dir[1];
		if (is_inside_board(new_file, new_rank)) {
			PPiece* piece = board[new_rank][new_file].piece;
			if (piece->piece_type == PieceType::Empty ||
				piece->color != board[rank][file].piece->color) {

				add_move(new_file, new_rank, moves);
			}
		}
	}
}

void Position::printBoard() {
	for (int i = 0; i < 10; i++) {
		QString line = "";
		for (int j = 0; j < 9; j++) {
			if (board[i][j].piece->piece_type == PieceType::Empty)
				line += "0";
			else
				line += QString::number((int)board[i][j].piece->piece_type + 1);
			line += " ";
		}
		qDebug() << line;
	}
}
