#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>
#include "glut.h"
//카메라 위치를 나타냄.
static GLfloat eyeX = 1; static GLfloat eyeY = 5; static GLfloat eyeZ = 12; 
//카메라가 바라보는 점.
static GLfloat centerX = 0.0f; static GLfloat centerY = 0.0f; static GLfloat centerZ = 0.0f; 
//카메라 상향 벡터 VUV에 해당.
static GLfloat ux = 0.0f; static GLfloat uy = 1.0f; static GLfloat uz = 0.0f;
//PiolotView 변화 값
static GLfloat roll = 0; static GLfloat pitch = 0; static GLfloat yaw = 0;
//벡터 클래스
class CVector3D
{
public:
	float x, y, z;
	//좌표 초기화
	CVector3D(){
		x = y = z = 0;
	}
	CVector3D(float _x, float _y, float _z)
	{
		x = _x, y = _y, z = _z;
	}

	float size()
	{
		return (sqrtf(x*x + y*y + z*z));
	}

	//현재 좌표들을 사이즈로 나눈다. (유닛백터화 시킴)
	void unitVectorize(void)
	{
		float fSize = this->size();
		x /= fSize;
		y /= fSize;
		z /= fSize;

	}
};
//좌표외적
//------------------------------------------------------
//   두 벡터 클래스형을 받아서 대수적으로 외적을 구한다.
//   ex) C = A x B = (a2b3-a3b2, a3b1 - a1b3, a1b2 - a2b1)
//------------------------------------------------------
CVector3D crossProduct(CVector3D a, CVector3D b)
{

	CVector3D c((a.y*b.z - a.z*b.y), (a.z*b.x - a.x*b.z), (a.x*b.y - a.y*b.x));
	return c;
}
//행성 사이즈
void PolarView(GLfloat radius, GLfloat elevation, GLfloat azimuth, GLfloat twist)
{
	glTranslatef(0.0, 0.0, -radius);
	glRotatef(-twist, 0.0, 0.0, 1.0);
	glRotatef(-elevation, 1.0, 0.0, 0.0);
	glRotatef(-azimuth, 0.0, 1.0, 0.0);
}
void PilotView(GLfloat roll, GLfloat pitch, GLfloat yaw)
{
	glRotatef(-roll, 0.0, 0.0, 1.0);
	glRotatef(-pitch, 1.0, 0.0, 0.0);
	glRotatef(-yaw, 0.0, 1.0, 0.0);
	glTranslatef(-0.2, -0.2, -0.2);
}
struct Size{
	float SunSize = 1.0f; float MercurySize = 0.2f;
	float VenusSize = 0.2f; float EarthSize = 0.2f;
	float MarsSize = 0.2f; float JupiterSize = 0.2f;
	float SaturnSize = 0.2f; float UranusSize = 0.2f;
	float NeptuneSize = 0.2f;
}size;
//행성 속도
struct Speed{
	float MercurySpeed = 0; 	float VenusSpeed = 0;
	float EarthSpeed = 0; float MoonSpeed = 0;
	float MarsSpeed = 0;	float JupiterSpeed = 0;
	float SaturnSpeed = 0;	float UranusSpeed = 0;
	float NeptuneSpeed = 0;
}speed;
void MyTimer(int Value)
{
	speed.MercurySpeed += 1;	speed.VenusSpeed += 2;
	speed.EarthSpeed += 0.5; speed.MoonSpeed += 2;
	speed.MarsSpeed += 4;	speed.JupiterSpeed += 2;
	speed.SaturnSpeed += 6;	speed.UranusSpeed += 1;
	speed.NeptuneSpeed += 9;
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}


//궤적 그리기
void drawCircle(float size)
{
	glBegin(GL_LINE_STRIP);
	for (float x = size; x >= -size; x -= 0.001f)
	{
		float z = sqrt(size * size - x*x);
		glVertex3f(x, 0.0f, z);
	}
	for (float x = -size; x <= size; x += 0.001f)
	{
		float z = sqrt(size * size - x*x);
		glVertex3f(x, 0.0f, -z);
	}
	glEnd();
	glFlush();

}
//행성 이름 써주기
void drawText(char *str)
{
	char *s = str;
	glRasterPos3f(-0.1f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < strlen(str); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}
//조명 설정
void InitLight()
{
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
void MyReshpae(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void specialKeyboard1(int key, int x, int y)
{
	//VPN변화없이 이동시 이동하는 단위
	const float nUnitOfMove = 0.5f;
	CVector3D Z((eyeX - centerX), (eyeY - centerY), (eyeZ - centerZ));
	Z.unitVectorize(); //유닛벡터화시킨다.
	CVector3D VUP(0, 1, 0);
	CVector3D X = crossProduct(VUP, Z);
	X.unitVectorize();
	CVector3D Y = crossProduct(X, Z);
	Y.unitVectorize();
	switch (key)
	{
	case GLUT_KEY_UP: //위로 이동
		cout << "VPN 변화 없이 위로 이동" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX -= Y.x*nUnitOfMove;
		eyeY -= Y.y*nUnitOfMove;
		eyeZ -= Y.z*nUnitOfMove;
		centerX -= Y.x*nUnitOfMove;
		centerY -= Y.y*nUnitOfMove;
		centerZ -= Y.z*nUnitOfMove;
		break;
	case GLUT_KEY_DOWN:
		cout << "VPN 변화 없이 아래로 이동" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX += Y.x*nUnitOfMove;
		eyeY += Y.y*nUnitOfMove;
		eyeZ += Y.z*nUnitOfMove;
		centerX += Y.x*nUnitOfMove;
		centerY += Y.y*nUnitOfMove;
		centerZ += Y.z*nUnitOfMove;
		break;
	case GLUT_KEY_RIGHT:
		cout << "VPN 변화 없이 우로이동" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX += X.x*nUnitOfMove;
		eyeY += X.y*nUnitOfMove;
		eyeZ += X.z*nUnitOfMove;
		centerX += X.x*nUnitOfMove;
		centerY += X.y*nUnitOfMove;
		centerZ += X.z*nUnitOfMove;
		break;
	case GLUT_KEY_LEFT: //좌로 이동
		cout << "VPN 변화 없이 좌로이동" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX -= X.x*nUnitOfMove;
		eyeY -= X.y*nUnitOfMove;
		eyeZ -= X.z*nUnitOfMove;
		centerX -= X.x*nUnitOfMove;
		centerY -= X.y*nUnitOfMove;
		centerZ -= X.z*nUnitOfMove;
		break;
	}
}
void keyboard1(unsigned char key, int x, int y) //키보드 버튼 조작
{


	const float ZoomValue = 0.01f;
	static float newEyeX, newEyeY, newEyeZ;
	static float newCenterX, newCenterY, newCenterZ;
	switch (key)
	{

	case'w':case'W'://줌인
		cout << "줌인" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX += ZoomValue*(centerX - eyeX);
		eyeY += ZoomValue*(centerY - eyeY);
		eyeZ += ZoomValue*(centerZ - eyeZ);
		centerX += ZoomValue*(centerX - eyeX);
		centerY += ZoomValue*(centerY - eyeY);
		centerZ += ZoomValue*(centerZ - eyeZ);
		break;
	case's':case'S'://줌아웃
		cout << "줌아웃" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX -= ZoomValue*(centerX - eyeX);
		eyeY -= ZoomValue*(centerY - eyeY);
		eyeZ -= ZoomValue*(centerZ - eyeZ);
		centerX -= ZoomValue*(centerX - eyeX);
		centerY -= ZoomValue*(centerY - eyeY);
		centerZ -= ZoomValue*(centerZ - eyeZ);
		break;

	case'f':case'F'://y축 CCW기준 회전
		cout << "카메라가 물체중심 y축 CCW방향 0.1rad만큼 이동회전" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		newEyeX = (eyeX - centerX)*cosf(0.1f) - (eyeZ - centerZ)*sinf(0.1f) + centerX;
		newEyeZ = (eyeX - centerX)*sinf(0.1f) + (eyeZ - centerZ)*cosf(0.1f) + centerZ;
		eyeX = newEyeX;
		eyeZ = newEyeZ;
		break;
	case'c':case'C'://y축 기준 CW 회전
		cout << "카메라가 물체중심 y축 CW방향 0.1rad만큼 이동회전" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		newEyeX = (eyeX - centerX)*cosf(-0.1f) - (eyeZ - centerZ)*sinf(-0.1f) + centerX;
		newEyeZ = (eyeX - centerX)*sinf(-0.1f) + (eyeZ - centerZ)*cosf(-0.1f) + centerZ;
		eyeX = newEyeX;
		eyeZ = newEyeZ;
		break;
		//카메라 제자리 회전
	case'd':case'D'://Y축 기준 회전
		cout << "카메라 제자리 회전(Y축 중심)" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		newCenterX = (eyeX - centerX)*-cosf(0.01f) - (eyeZ - centerZ)*-sinf(0.01f) + eyeX;
		newCenterZ = (eyeX - centerX)*-sinf(0.01f) + (eyeZ - centerZ)*-cosf(0.01f) + eyeZ;
		centerX = newCenterX;
		centerZ = newCenterZ;

		break;
	case'a':case'A'://Y축 기준 회전
		cout << "카메라 제자리 회전(Y축 중심)" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		newCenterX = (eyeX - centerX)*-cosf(-0.01f) - (eyeZ - centerZ)*-sinf(-0.01f) + eyeX;
		newCenterZ = (eyeX - centerX)*-sinf(-0.01f) + (eyeZ - centerZ)*-cosf(-0.01f) + eyeZ;
		centerX = newCenterX;
		centerZ = newCenterZ;
		break;

	case'k':case'K'://Z축 기준 회전
		cout << "카메라 제자리 회전(Z축 중심)" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		newCenterX = (eyeX - centerX)*(-cosf(0.01f)) - (eyeY - centerY)*(-sinf(0.01f)) + eyeX;
		newCenterY = (eyeX - centerX)*(-sinf(0.01f)) + (eyeZ - centerY)*(-cosf(0.01f)) + eyeY;
		centerX = newCenterX;
		centerY = newCenterY;
		break;

	case'i':case'I':
		cout << "Roll 회전 " << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		roll += 1.0f;
		break;
	case'o':case'O':
		cout << "Pitch 회전 " << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		pitch += 1.0f;
		break;
	case'u':case'U':
		cout << "Yaw 회전 " << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		yaw += 1.0f;
		break;

	case'p':case'P'://초기화
		cout << "초기화" << endl;
		printf("(eyeX:%.2f,eyeY:%.2f,eyeZ:%.2f,centerX:%.2f,centerY:%.2f,centerZ:%.2f,ux:%.2f,uy:%.2f,uz:%.2f\n)", eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
		eyeX = 1;
		eyeY = 5;
		eyeZ = 12;
		centerX = 0.0f;
		centerY = 0.0f;
		centerZ = 0.0f;
		ux = 0.0f;
		uy = 1.0f;
		uz = 0.0f;
		roll = 0;
		pitch = 0;
		yaw = 0;
		break;
	}
}
void MyDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//zoom과 회전에 관련된 일을 함.
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, ux, uy, uz);
	PilotView(roll, pitch, yaw);
	//Sun태양
	glColor3f(1, 1, 0);			//yellow
	glutSolidSphere(size.SunSize, 50, 50);
	drawText("Sun");
	//Line_STRIP 공간상의 점과 점을 선으로 연결한 원
	drawCircle(2.0f);	drawCircle(4.0f);
	drawCircle(6.0f);	drawCircle(8.0f);
	drawCircle(10.0f);	drawCircle(12.0f);
	drawCircle(14.0f);	drawCircle(16.0f);
	//Mercury수성
	glPushMatrix();
	glRotatef(speed.MercurySpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 2.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(size.MercurySize, 50, 50);
	drawText("Mecury");
	glPopMatrix();
	//Venus금성
	glPushMatrix();
	glRotatef(speed.VenusSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 4.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(size.VenusSize, 50, 50);
	drawText("Venus");
	glPopMatrix();
	//Earth지구
	glPushMatrix();
	glRotatef(speed.EarthSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 6.0);
	drawCircle(1.0f);
	glColor3f(0.0, 1.0, 1.0);
	drawText("Earth");
	glutSolidSphere(size.EarthSize, 50, 50);
	//달 삽입
	glPushMatrix();
	glRotatef(speed.MoonSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 1.0);
	glutSolidSphere(0.1, 50, 50);
	drawText("Moon");
	glPopMatrix();
	glPopMatrix();
	//Mars화성
	glPushMatrix();
	glRotatef(speed.MarsSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 8.0);
	glColor3f(1.0, 0.0, 1.0);
	glutSolidSphere(size.MarsSize, 50, 50);
	drawText("Mars");
	glPopMatrix();
	//Jupiter목성
	glPushMatrix();
	glRotatef(speed.JupiterSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 10.0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(size.JupiterSize, 50, 50);
	drawText("Jupiter");
	glPopMatrix();
	//Saturn토성
	glPushMatrix();
	glRotatef(speed.SaturnSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 12.0);
	glColor3f(0.0, 1.0, 1.0);
	glutSolidSphere(size.SaturnSize, 50, 50);
	drawText("Saturn");
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidTorus(0.35, 0.39, 10, 10);
	glPopMatrix();
	glPopMatrix();
	//Uranus천왕성
	glPushMatrix();
	glRotatef(speed.UranusSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 14.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(size.UranusSize, 50, 50);
	drawText("Uranus");
	glPopMatrix();
	//Neptune해왕성
	glPushMatrix();
	glRotatef(speed.NeptuneSpeed, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 16.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(size.NeptuneSize, 50, 50);
	drawText("Neptune");
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Solar System");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	InitLight();
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keyboard1);
	glutSpecialFunc(specialKeyboard1);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshpae);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}
