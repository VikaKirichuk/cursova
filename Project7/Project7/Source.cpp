#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;

const int boardSize = 8;

enum Player {
    PLAYER1,
    PLAYER2
 
};

enum Piece {
    EMPTY,
    PLAYER1_PIECE,
    PLAYER2_PIECE,
    PLAYER1_KING,
    PLAYER2_KING,
};

struct Move {
    int fromRow; 
    int fromCol;
    int toRow;
    int toCol;
};

class CheckersGame {
private:
    vector<vector<Piece>> board;
    Player currentPlayer;

public:
    CheckersGame() : board(boardSize, vector<Piece>(boardSize, EMPTY)), currentPlayer(PLAYER1) {
        initializeBoard();
    }

    void initializeBoard() { // ��������� ����
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (((i + j) & 1) == 1) {
                    if (i < 3) {
                        board[i][j] = PLAYER1_PIECE;
                    }
                    else if (i > 4) {
                        board[i][j] = PLAYER2_PIECE;
                    }
                }
            }
        }
    }

    void printBoard() {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                switch (board[i][j]) {
                case EMPTY:
                    cout << ".";
                    break;
                case PLAYER1_PIECE:
                    cout << "X";
                    break;
                case PLAYER2_PIECE:
                    cout << "O";
                    break;
                }
                cout << " ";
            }
            cout << endl;
        }
    }

    bool isValidCell(int row, int col) {
        return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
     }

    bool hasCycle(int v, vector<vector<int>>& graph, vector<int>& visited, vector<int>& path, int& flag) {
        if (flag == 1) return true; // ���� ��� ������� ����, �� ��������� true
        else {
            visited[v] = 1; // ��������� ������� �� �������
            path.push_back(v); // ������ �� � ������� ������ �����
            for (int i = 0; i < graph[v].size(); i++) {
                int to = graph[v][i]; // �������� ������� �����
                if (visited[to] == 1) { // ���� �� �� ��� �������, ��� �� ������ � ��, �������, �� ������� ����
                    path.push_back(to); // ������ �������� ������� � ������� ������ �����
                    flag = 1; // ������� ���������, �� �� ������� ����
                    return true;
                }
                else if (visited[to] == 0 && hasCycle(to, graph, visited, path, flag)) {

                    return true;

                }
            }
            visited[v] = 2; // ���� �� ������� ����, �� �������� � �������
            path.pop_back();
        }
        return false;
    }

    bool isValidMove(const Move& move) {


        int fromRow = move.fromRow;
        int fromCol = move.fromCol;
        int toRow = move.toRow;
        int toCol = move.toCol;

        // ��������, �� ������ �������� ��������� �������
        Piece currentPiece = board[fromRow][fromCol];
        Player currentPlayer = getCurrentPlayer();

        if ((currentPiece == PLAYER1_PIECE || currentPiece == PLAYER1_KING) && currentPlayer != PLAYER1) {
            return false;
        }
        else if ((currentPiece == PLAYER2_PIECE || currentPiece == PLAYER2_KING) && currentPlayer != PLAYER2) {
            return false;
        }

        // ���������� ����� ��� �������� �����
        vector<vector<int>> graph(boardSize * boardSize);
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (isValidCell(i, j)) {

                    if (isValidCell(i - 1, j - 1)) {
                        graph[i * boardSize + j].push_back((i - 1) * boardSize + (j - 1));
                    }
                    if (isValidCell(i - 1, j + 1)) {
                        graph[i * boardSize + j].push_back((i - 1) * boardSize + (j + 1));
                    }
                    if (isValidCell(i + 1, j - 1)) {
                        graph[i * boardSize + j].push_back((i + 1) * boardSize + (j - 1));
                    }
                    if (isValidCell(i + 1, j + 1)) {
                        graph[i * boardSize + j].push_back((i + 1) * boardSize + (j + 1));
                    }

                }
            }
        }

        // ����������� ��� DFS
        int flag = 0;
        vector<int> visited(boardSize * boardSize, 0);
        vector<int> path;

        // �������� ����� ����� ���������� ������ ����
        if (hasCycle(fromRow * boardSize + fromCol, graph, visited, path, flag)) {
            return false; // ���� � ����, ��� ������������
        }


        // �������: ��������, �� ����� ������ ���� �� ������ �������
        if (board[toRow][toCol] != EMPTY) {
            return false; // ������� �� �������
        }

        // �������: ��������, �� ����� ����� ����� ���������� (��������� �� ������� �� �� ������)
        if (abs(toRow - fromRow) == 2) {
            int capturedRow = (toRow + fromRow) / 2;
            int capturedCol = (toCol + fromCol) / 2;
            Piece capturedPiece = board[capturedRow][capturedCol];

            if (currentPlayer == PLAYER1 && (capturedPiece == PLAYER2_PIECE || capturedPiece == PLAYER2_KING)) {
                // ����� ����� ����� ����������
                // ������� ����� ��������� ����� ���������� (capturedPiece) � �����
                return true;
            }
            else if (currentPlayer == PLAYER2 && (capturedPiece == PLAYER1_PIECE || capturedPiece == PLAYER1_KING)) {
                // ����� ����� ����� ����������
                // ������� ����� ��������� ����� ���������� (capturedPiece) � �����
                return true;
            }
        }

        // ���� �� �������� ������� ������, ��������� true
        return true;
    }

   

    bool isGameOver() {
        bool player1HasPieces = false;
        bool player2HasPieces = false;

        // �������� �������� ����� � �������
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] == PLAYER1_PIECE || board[i][j] == PLAYER1_KING) {
                    player1HasPieces = true;
                }
                else if (board[i][j] == PLAYER2_PIECE || board[i][j] == PLAYER2_KING) {
                    player2HasPieces = true;
                }
            }
        }


        if (!player1HasPieces ) {
            cout << "Player 1 wins!" << endl;
            return true;
        }
        else if (!player2HasPieces ) {
            cout << "Player 2 wins!" << endl;
            return true;
        }

        return false;
    }


    void switchPlayer() {
        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    Player getCurrentPlayer() const {
        return currentPlayer;
    }


    void makeMove(const Move& move) {
        int fromRow = move.fromRow;
        int fromCol = move.fromCol;
        int toRow = move.toRow;
        int toCol = move.toCol;

        // ��������� ��� �� ������ ���� ���
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = EMPTY;

        // ������� ����� ��� ���������� �� � ������
        if (abs(toRow - fromRow) == 2) {
            // ��������� ����� ����������
            int capturedRow = (toRow + fromRow) / 2;
            int capturedCol = (toCol + fromCol) / 2;
            board[capturedRow][capturedCol] = EMPTY;
        }

        
        if ((toRow == 0 && getCurrentPlayer() == PLAYER1 && board[toRow][toCol] == PLAYER1_PIECE) ||
            (toRow == boardSize - 1 && getCurrentPlayer() == PLAYER2 && board[toRow][toCol] == PLAYER2_PIECE)) {
            // ������������ �� �����
            board[toRow][toCol] = (getCurrentPlayer() == PLAYER1) ? PLAYER1_KING : PLAYER2_KING;
        }
    }

};

class CheckersAI {
public:
    static Move generateMove(const CheckersGame& game, Player currentPlayer, bool isHardLevel) {
        std::vector<Move> possibleMoves = getAvailableMoves(game, currentPlayer);

        if (possibleMoves.empty()) {
            return Move{};
        }

        if (!isHardLevel) {
            srand(static_cast<unsigned int>(time(0)));
            int randomIndex = rand() % possibleMoves.size();
            return possibleMoves[randomIndex];
        }

        // ������������� �������� ������ � ������� ��� ���������� ������� ����
        Move bestMove = depthFirstSearch(game, currentPlayer, 3);  // ������� ������ - 3

        return bestMove;
    }

private:
    static Move depthFirstSearch(const CheckersGame& game, Player currentPlayer, int depth) {
        std::vector<Move> possibleMoves = getAvailableMoves(game, currentPlayer);

        if (possibleMoves.empty() || depth == 0) {
            // ������� ����������� ������� ��� ���� ��������� ����, �������� ���� ���
            // � ��������� ������ ��� �� ������� (�������� ������� ������ - ���������� ���)
            srand(static_cast<unsigned int>(time(0)));
            int randomIndex = rand() % possibleMoves.size();
            return possibleMoves[randomIndex];
        }

        Move bestMove;
        int bestScore = std::numeric_limits<int>::min();

        for (const Move& move : possibleMoves) {
            // ������ ���
            CheckersGame newGame = game;
            newGame.makeMove(move);

            // ���������� ��������� ����� � ������� ��� ���������� ������
            Move opponentMove = depthFirstSearch(newGame, getOpponent(currentPlayer), depth - 1);

            // �������� ���� ��� (����� ������ ���� ������ ����)
            int score = evaluateBoard(newGame, currentPlayer) + evaluateMove(move);

            // ���� �������� ������� - ������������, ������� ��� � �������� �������
            // ���� �������� ������� - �����������, ������� ��� � ��������� �������
            if ((currentPlayer == Player::WHITE && score > bestScore) ||
                (currentPlayer == Player::BLACK && score < bestScore)) {
                bestScore = score;
                bestMove = move;
            }
        }

        return bestMove;
    }

    // ��������� ������ getAvailableMoves, evaluateBoard �� evaluateMove, �� ��� ������ ��� ���� ���.
    // getOpponent - �� ������ �������� ������� ��� ��������� �������� ������.
    // ������ ����� ����������� � ��������� �� ���������� ������ ���.
    // � �������� ������� �� ������ ��������������� �������� ������.
};



int main() {
    CheckersGame game;

    cout << "Choose mode:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs Computer" << endl;
    int mode;
    cin >> mode;

    if (mode == 1) {
        // ������� ����� ������
        while (!game.isGameOver()) {
            cout << "Current Player: " << (game.getCurrentPlayer() == PLAYER1 ? "Player 1" : "Player 2") << endl;
            game.printBoard();

            Move move;

            cout << "Enter your move (fromRow): ";
            cin >> move.fromRow;

            cout << "Enter your move (fromCol): ";
            cin >> move.fromCol;

            cout << "Enter your move (toRow): ";
            cin >> move.toRow;

            cout << "Enter your move (toCol): ";
            cin >> move.toCol;

            if (game.isValidMove(move)) {
                game.makeMove(move);
                game.switchPlayer();
            }
            else {
                cout << "Invalid move. Try again." << endl;
            }
        }
        /*int fromRow = move.fromRow;
        int fromCol = move.fromCol;
        int toRow = move.toRow;
        int toCol = move.toCol;*/

    }
    else if (mode == 2) {
        // ������� ����� ����'�����
        cout << "Choose difficulty level:" << endl;
        cout << "1. Easy" << endl;
        cout << "2. Hard" << endl;
        int level;
        cin >> level;

        while (!game.isGameOver()) {
            if (game.getCurrentPlayer() == PLAYER1) {
                // ճ� ������
                cout << "Current Player: Player 1" << endl;
                game.printBoard();

                Move move;
                cout << "Enter your move (fromRow fromCol toRow toCol): ";
                cin >> move.fromRow >> move.fromCol >> move.toRow >> move.toCol;

                if (game.isValidMove(move)) {
                    game.makeMove(move);
                    game.switchPlayer();
                }
                else {
                    cout << "Invalid move. Try again." << endl;
                }
            }
            else {
                // ճ� ����'�����
                cout << "Current Player: Computer" << endl;
                game.printBoard();

                Move aiMove = CheckersAI::generateMove(game, PLAYER2, level == 2);
                game.makeMove(aiMove);
                game.switchPlayer();
            }
        }
    }

    cout << "Game over!" << endl;
    return 0;
}


