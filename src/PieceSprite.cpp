# include "PieceSprite.h"

const map<color, vector<int>> PieceSprite::col2textrec = {
    {red, {30, 30, 30, 30}},
    {blue, {0, 30, 30, 30}},
    {green, {30, 0, 30, 30}},
    {yellow, {0, 0, 30, 30}}
};

const map<color, vector<int>> PieceSprite::col2selectedrec = {
    {red, {30, 90, 30, 30}},
    {blue, {0, 90, 30, 30}},
    {green, {30, 60, 30, 30}},
    {yellow, {0, 60, 30, 30}}
};

PieceSprite::PieceSprite(const Texture & t, int id, color c) : ClickableSprite(t){
    this->id = id;
    this->c = c;
    this->setTextureRect(IntRect(col2textrec.at(c).at(0), col2textrec.at(c).at(1), col2textrec.at(c).at(2), col2textrec.at(c).at(3)));
}


void PieceSprite::onClickAction(Window & container){

    ParchisGUI * gui = dynamic_cast<ParchisGUI*>(&container);

    if(clicked == true){
        Box origin = gui->model->getBoard().getPiece(this->getModelColor(), this->id);

        gui->model->movePiece(this->getModelColor(), this->id,  gui->last_dice);
        vector<tuple<color, int, Box>> last_moves = gui->model->getLastMoves();


        for (int i = 0; i < last_moves.size(); i++){
            Box dest = get<2>(last_moves[i]);
            vector<pair<color, int>> occupation = gui->model->boxState(dest);
            // Si en la casilla de destino solo hay una ficha (la que se está moviendo)
            if (occupation.size() == 1 || dest.type == home || dest.type == goal)
            {
                cout << "HOLAAAA" << endl;
                Vector2f animate_pos= Vector2f(gui->box3position(get<0>(last_moves[i]), get<1>(last_moves[i]), 0));

                //box2position.at(get<2>(last_moves[i]))[0].x, gui->box2position.at(get<2>(last_moves[i]))[0].y);
                
                Sprite *animate_sprite = &gui->pieces[get<0>(last_moves[i])][get<1>(last_moves[i])];
                SpriteAnimator animator = SpriteAnimator(*animate_sprite, animate_pos, 1000);
                gui->animations.push_back(animator);
            // Si en la casilla de destino hay ahora dos fichas, la que se mueve y una anterior
            }
            else
            {
                Vector2f animate_pos = Vector2f(gui->box3position(occupation.at(0).first, occupation.at(0).second, 1));
                Sprite *animate_sprite = &gui->pieces[occupation.at(0).first][occupation.at(0).second];
                SpriteAnimator animator = SpriteAnimator(*animate_sprite, animate_pos, 1000);
                gui->animations.push_back(animator);

                Vector2f animate_2_pos= Vector2f(gui->box3position(occupation.at(1).first, occupation.at(1).second, 2));
                Sprite *animate_2_sprite = &gui->pieces[occupation.at(1).first][occupation.at(1).second];
                SpriteAnimator animator_2 = SpriteAnimator(*animate_2_sprite, animate_2_pos, 1000);
                gui->animations.push_back(animator_2);
            }

            const vector<pair <color, int>> origin_occupation = gui->model->boxState(origin);
            cout << occupation.size() << " " << origin_occupation.size() << endl;
            // Si antes del movimiento había dos fichas en la casilla de origen (ahora solo queda una).
            if (origin_occupation.size() == 1){
                cout << "HOLOOOO" << endl;
                Vector2f animate_pos = Vector2f(gui->box3position(origin_occupation.at(0).first, origin_occupation.at(0).second, 0));
                //Vector2f animate_pos= Vector2f(gui->box2position.at(origin)[0].x, gui->box2position.at(origin)[0].y);
                Sprite *animate_sprite = &gui->pieces[origin_occupation.at(0).first][origin_occupation.at(0).second];
                SpriteAnimator animator = SpriteAnimator(*animate_sprite, animate_pos, 1000);
                gui->animations.push_back(animator);
            }

        }
    }
}

void PieceSprite::onEnableAction(Window & container){}

void PieceSprite::onSelectionAction(Window & container){}

void PieceSprite::onHoverAction(Window & container){
    if(hovered){
        this->setTextureRect(IntRect(col2selectedrec.at(c).at(0), col2selectedrec.at(c).at(1), col2selectedrec.at(c).at(2), col2selectedrec.at(c).at(3)));
    }
    else{
        this->setTextureRect(IntRect(col2textrec.at(c).at(0), col2textrec.at(c).at(1), col2textrec.at(c).at(2), col2textrec.at(c).at(3)));
    }
}