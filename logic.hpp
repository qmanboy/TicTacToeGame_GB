#pragma once

#include <cstdint>
#include "field.hpp"

enum class CellState { //статус клетки
     Empty, X, O
};

enum class PlayerSign { //знак игрока
    X, O
};

struct Pos_move { //структура для возврата введеной позиции клетки
    size_t x,y;
};

size_t FIELD_WIDTH = query_field_size(); //запрос размеров поля, запись в FIELD_WIDTH

CellState* Fieldptr = new CellState[FIELD_WIDTH * FIELD_WIDTH]; //выделение памяти

void init_field(CellState * field) { //инициализация выделенной памяти
    for (size_t i = 0; i < (FIELD_WIDTH * FIELD_WIDTH); i++) {
        field[i] = CellState::Empty;
    }
}    

enum class TurnOutCome { //статус игры
    CONTINUE,
    DRAW,
    X_WIN,
    O_WIN
};

bool is_valid_pos(size_t row, size_t col) { //проверка правильности введеной позиции
    return row < FIELD_WIDTH && col < FIELD_WIDTH; //столбцы и строки меньше размера поля - true
}

CellState get_cell(CellState* field, size_t row, size_t col) {  //проверка состояния клетки, ссылка на массив клеток поля, значения строк и столбцов
    if (is_valid_pos(row, col)) { //если позиция корректна
        return field[col * FIELD_WIDTH + row]; //возвращаем состояние клетки-элемента массива клеток поля 
    }
    return CellState::Empty; //возвращает пустую клетку
}

bool set_cell(CellState* field, size_t row, size_t col, CellState state) { //установить значение в клетку
    if (is_valid_pos(row,col) && get_cell(field, row, col) == CellState::Empty) { //проверка на корректность введеных данных и пустоту клетки
        field[col * FIELD_WIDTH + row] = state; //если ок - пишем в клетку
        return true;
    }
    return false; 
}

bool is_draw(CellState* field) { //проверка на ничью
    for (size_t i=0; i < FIELD_WIDTH*FIELD_WIDTH; i++)
        {
            if (field[i]==CellState::Empty)
                return false;
        }
    return true;
}

CellState is_line_full(CellState* field, size_t start_x, size_t start_y, short dx, short dy) { //проверка заполненности линий
    
    size_t x = start_x, y = start_y;
    CellState first_cell = get_cell(field, x, y); //получаем первую клетку линии
    if (first_cell == CellState::Empty) 
       return CellState::Empty;
    while (is_valid_pos(x + dx, y + dy)) { //пока позиция входит в поле, сравниваем клетку с предыдущей
        x +=dx;
        y +=dy;
        if (get_cell(field, x, y) != first_cell) {//если не равна, возвращаем пустую
            return CellState::Empty;
        }
    } 
    return first_cell; //если все равны то возвращаем выигрышное значение
}

TurnOutCome check_turn_outcome(CellState* field) { //макрос заменяющий возвращение результата проверки линий по входным данным
    #define CHECK_LINE(start_x, start_y, dx, dy) \
    if (CellState res = is_line_full(field, start_x, start_y, dx, dy); res != CellState::Empty)     \
        {                                                                                               \
        return res == CellState::X ? TurnOutCome::X_WIN : TurnOutCome::O_WIN;                      \
        } 
    
    for (size_t i = 0; i < FIELD_WIDTH; i++) {
        //check row
        CHECK_LINE(0, i, 1, 0);
        //check col
        CHECK_LINE(i, 0, 0, 1);
    }

    CHECK_LINE(0, 0, 1, 1);
    CHECK_LINE((FIELD_WIDTH-1), 0, -1, 1);

#undef CHECK_LINE 

    if (is_draw(field)) {
        return TurnOutCome::DRAW;
    }
return TurnOutCome::CONTINUE;
}

void print_cell(CellState cell) { //печать в поле введеной клетки
    switch (cell)
    {
    case CellState::X:
        std::cout << "X";
        break;
    case CellState::O:
        std::cout << "O";
        break;    
    default:
    std::cout << "_";
        break;
    }    
}

void print_field(CellState* field) { //вывод игрового поля после хода
    
    for (size_t col= 0; col < FIELD_WIDTH; col++) {
        std::cout << "|";
        for (size_t row = 0; row < FIELD_WIDTH; row++) {
            print_cell(get_cell(field, row, col));
            if (row == FIELD_WIDTH-1)
                std::cout << "|\n";
            else 
                std::cout << "|"; 
        }           
    }
    std::cout << "\n"; 
}

void print_winner(PlayerSign cur, PlayerSign hum) { //проверка и вывод победителя игры
    if (cur == hum)  
        std::cout << "You win! Congrats!\n";
    else  
        std::cout << "Game over. AI win.\n";
}

void print_game_outcome(TurnOutCome outcome, PlayerSign current, PlayerSign humansign) { //вывод результата игры
    switch (outcome)
    {
    case TurnOutCome::DRAW:
        std::cout << "It's a draw!\n";
        break;
    case TurnOutCome::X_WIN:
        print_winner(current, humansign); 
        break;
    case TurnOutCome::O_WIN:
        print_winner(current, humansign);
        break;    
    
    default:
        break;
    delete [] Fieldptr;//освобождение памяти
    }
} 

CellState sign_to_cell(PlayerSign sign) {  //определение статуса клетки по сигну игрока
    return sign == PlayerSign::X ? CellState::X : CellState::O;
}

PlayerSign next_player(PlayerSign current) { //смена хода
    return current == PlayerSign::X ? PlayerSign::O : PlayerSign::X;
}

bool query_replay() { //запрос на повтор игры
    char answer;
    std::cout << "Replay? (y - yes, any key - exit): ";
    std::cin >> answer;
    std::cout << "\n";
    if (answer == 'y')
        return true;
    else
        return false;
}

