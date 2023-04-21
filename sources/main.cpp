#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <raylib.h>

class Ball {
public:
    int posX, posY;
    int velX, velY;
    int radius;


    void Draw() {
        DrawCircle(posX, posY, radius, RED);
    }

    void Update() {
        posX += velX;
        posY += velY;

        if (posY + radius >= GetScreenHeight() || posY - radius <= 0) {
            velY *= -1;
        }

        if (posX + radius >= GetScreenWidth() || posX - radius <= 0) {
            velX *= -1;
        }
    }
    
};

class Paddle {
public:
    int posX, posY;
    int sizeX, sizeY;
    int speed;

    int score;

    void Draw() {
        DrawRectangle(posX, posY, sizeX, sizeY, RED);
    }

    void Update() {

        if (IsKeyDown(KEY_UP))
        {
            posY -= 6;
        }
        else if (IsKeyDown(KEY_DOWN)) 
        {
            posY += 6;  
        }

        if (posY <= 0) 
        {
            posY = 0;
        }

        if (posY + sizeY > GetScreenHeight()) 
        {
            posY = GetScreenHeight() - sizeY;
        }
    }
    
};

class CPUPaddle : public Paddle {
public:    

    void Update(int ballPosY)
    {

        if (posY + sizeY / 2 > ballPosY) 
        {
            posY -= speed;
        }

         if (posY + sizeY / 2 < ballPosY) 
        {
            posY += speed;
        }


        if (posY <= 0) 
        {
            posY = 0;
        }

        if (posY + sizeY > GetScreenHeight()) 
        {
            posY = GetScreenHeight() - sizeY;
        }
    }

};

Ball ball;
Paddle player;
CPUPaddle ai;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 800;

    ball.radius = 20;
    ball.posX = screenWidth / 2;
    ball.posY = screenHeight / 2;
    ball.velX = 5;
    ball.velY = 5;

    player.sizeX = 40;
    player.sizeY = 120;
    player.posX = 10;
    player.posY = screenHeight / 2 - player.sizeY / 2;
    player.speed = 0;
    

    ai.sizeX = 40;
    ai.sizeY = 120;
    ai.posX = screenWidth - ai.sizeX - 10;
    ai.posY = screenHeight / 2 - player.sizeY / 2;
    ai.speed = 6;

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
 
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        ball.Update();
        player.Update();
        ai.Update(ball.posY);
        //----------------------------------------------------------------------------------

        if (CheckCollisionCircleRec(Vector2{(float)ball.posX, (float)ball.posY}, ball.radius, Rectangle{(float)player.posX, (float)player.posY, (float)player.sizeX, (float)player.sizeY})) 
        {
            ball.velX *= -1;
            player.score += 1; // this isnt the best but this is the easiest way
        }

        if (CheckCollisionCircleRec(Vector2{(float)ball.posX, (float)ball.posY}, ball.radius, Rectangle{(float)ai.posX, (float)ai.posY, (float)ai.sizeX, (float)ai.sizeY})) 
        {
            ball.velX *= -1;
            ai.score += 1;  // this isnt the best but this is the easiest way
        }
 
        // Draw
        //----------------------------------------------------------------------------------

        BeginDrawing();
 
            ClearBackground(RAYWHITE);

            DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, RED);
            ball.Draw();
            player.Draw();
            ai.Draw();
 
            DrawText(TextFormat("%i", player.score), screenWidth / 4 - 20, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("%i", ai.score), 3 * screenWidth / 4 - 20, 20, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
 
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
 
    return 0;
}