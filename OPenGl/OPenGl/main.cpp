#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
using namespace std;

// 카메라 관련 전역 변수
double cameraRadius = 50.0f;        // 카메라와 원점(타겟) 사이 거리
double cameraTheta = 45.0f;         // 수평 각도 (y축 기준, degree)
double cameraPhi = 45.0f;           // 수직 각도 (위에서 내려다보는 각, degree)
int lastX = 0, lastY = 0;           // 마지막 마우스 위치
bool mouseLeftDown = false;         // 왼쪽 버튼 눌림 여부

// 삼각형/직선 데이터
GLfloat triangleVertices[3][3] = {
    {1.0f, 0.0f, 0.0f}, // A
    {0.0f, 1.0f, 0.0f}, // B
    {0.0f, 0.0f, 1.0f}  // C
};

GLfloat lineVertices[2][3] = {
    {0.0f, 0.0f, 0.0f}, // D
    {1.0f, 1.0f, 1.0f}  // E
};

void drawTriangle(GLfloat vertices[3][3]) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // A: 빨강
    glVertex3fv(vertices[0]);
    glColor3f(0.0f, 1.0f, 0.0f); // B: 초록
    glVertex3fv(vertices[1]);
    glColor3f(0.0f, 0.0f, 1.0f); // C: 파랑
    glVertex3fv(vertices[2]);
    glEnd();
}

void drawLine(GLfloat vertices[2][3]) {
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // 흰색
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glEnd();
}

// 마우스 드래그로 카메라 회전
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = true;
            lastX = x;
            lastY = y;
        }
        else if (state == GLUT_UP) {
            mouseLeftDown = false;
        }
    }
    if (state == GLUT_DOWN) {
        if (button == 3) { // 휠 업
            cameraRadius *= 0.9f;
            if (cameraRadius < 1.0f) cameraRadius = 1.0f;
            glutPostRedisplay();
        }
        else if (button == 4) { // 휠 다운
            cameraRadius *= 1.1f;
            if (cameraRadius > 50.0f) cameraRadius = 50.0f;
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    if (mouseLeftDown) {
        double dx = double(x - lastX);
        double dy = double(y - lastY);

        cameraTheta += dx * 0.1f; // 수평 회전
        cameraPhi -= dy * 0.1f; // 수직 회전

        if (cameraPhi < 1.0f) cameraPhi = 0.1f;
        if (cameraPhi > 179.0f) cameraPhi = 179.9f;

        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

// 3D 위치에 텍스트 출력
void draw_Text3d(const char* text, float x, float y, float z) {
    glRasterPos3f(x, y, z); // 3D 위치 지정
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    double theta = cameraTheta * 3.141592f / 180.0f;
    double phi = cameraPhi * 3.141592f / 180.0f;
    double camX = cameraRadius * sinf(phi) * cosf(theta);
    double camY = cameraRadius * cosf(phi);
    double camZ = cameraRadius * sinf(phi) * sinf(theta);

    gluLookAt(
        camX, camY, camZ,  // 카메라 위치
        0.0, 0.0, 0.0,     // 타겟(원점)
        0.0, 1.0, 0.0      // 업 벡터
    );

    glBegin(GL_LINES);
    // X축 (빨간색)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-50.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);
    // Y축 (초록색)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    // Z축 (파란색)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -50.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();

    drawTriangle(triangleVertices);
    drawLine(lineVertices);

    glColor3f(1.0f, 1.0f, 1.0f);
    draw_Text3d("O", 0.0f, 0.0f, 0.0f);
    draw_Text3d("A", triangleVertices[0][0], triangleVertices[0][1], triangleVertices[0][2]);
    draw_Text3d("B", triangleVertices[1][0], triangleVertices[1][1], triangleVertices[1][2]);
    draw_Text3d("C", triangleVertices[2][0], triangleVertices[2][1], triangleVertices[2][2]);
    draw_Text3d("D", lineVertices[0][0], lineVertices[0][1], lineVertices[0][2]);
    draw_Text3d("E", lineVertices[1][0], lineVertices[1][1], lineVertices[1][2]);

    draw_Text3d("X", cameraRadius / 1.95f, 0.0f, 0.0f);
    draw_Text3d("Y", 0.0f, cameraRadius / 2.8f, 0.0f);
    draw_Text3d("Z", 0.0f, 0.0f, cameraRadius / 1.95f);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 0.1, 100.0);
}

int main(int argc, char** argv) {
    double Tri[3][3] = { 0 };
    char asc = 'A';
    cout << "삼각형 ABC 는 점 A, 점 B, 점 C 로 이루어져 있습니다." << endl;
    for (int i = 0; i < 3; i++) {
        cout << "점" << asc << " 의 x, y, z 값을 순서대로 입력하시오. ";
        cin >> triangleVertices[i][0] >> triangleVertices[i][1] >> triangleVertices[i][2];
        Tri[i][0] = triangleVertices[i][0];
        Tri[i][1] = triangleVertices[i][1];
        Tri[i][2] = triangleVertices[i][2];
        asc++;
    }
    double Bang[4] = { 0 };
    for (int i = 0; i < 3; i++) {
        int m = i - 1, p = i + 1;
        if (m < 0)
            m = 2;
        if (p > 2)
            p = 0;
        Bang[i] += (Tri[p][1] - Tri[p][0]) * (Tri[m][2] - Tri[m][0]);
        Bang[i] -= (Tri[m][1] - Tri[m][0]) * (Tri[p][2] - Tri[p][0]);
    }
    Bang[3] = Bang[0] * Tri[0][0] + Bang[1] * Tri[0][1] + Bang[2] * Tri[0][2];
    cout << "삼각형 ABC이 포함된 의 평면의 방정식은" << endl;
    cout << Bang[0] << " x + " << Bang[1] << " y + " << Bang[2] << " z = " << Bang[3] << endl;
    cout << "입니다." << endl;
    double a = Bang[0], b = Bang[1], c = Bang[2], d = Bang[3];

    cout << endl;
    double Segment[2][3] = { 0 };
    cout << "선분 DE 는 점 D, 점 E 로 이루어져 있다." << endl;
    for (int i = 0; i < 2; i++) {
        cout << "점" << asc << "의 x, y, z 값을 순서대로 입력하시오. ";
        cin >> lineVertices[i][0] >> lineVertices[i][1] >> lineVertices[i][2];
        Segment[i][0] = lineVertices[i][0];
        Segment[i][1] = lineVertices[i][1];
        Segment[i][2] = lineVertices[i][2];
        asc++;
    }
    cout << endl << endl;
    double x = 0, y = 0, z = 0;
    double alpha = Segment[1][0] - Segment[0][0], beta = Segment[1][1] - Segment[0][1], gamma = Segment[1][2] - Segment[0][2];
    if (alpha * a + beta * b + gamma * c == 0)
        cout << "삼각형 ABC 와 선분 DE 의 교점이 없거나 무수히 많습니다." << endl;
    else {
        x = alpha * (d - b * Segment[0][1] - c * Segment[0][2]) + beta * Segment[0][0] * b + gamma * Segment[0][0] * c;
        x = x / (alpha * a + beta * b + gamma * c);
        cout << "삼각형 ABC 와 선분 DE 의 교점의 x 좌표는 " << x << " 입니다." << endl;
    }

    cout << endl << endl;
    cout << "x 축, y 축, z 축과 선분 DE 는 흰색으로 그렸으며, " << endl;
    cout << "삼각형 ABC 의 점 A 는 빨간색, 점 B 는 초록색, 점 C 는 파란색으로 그렸습니다." << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Triangle and Line");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // 마우스, 드래그 콜백 등록
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();

    return 0;
}
