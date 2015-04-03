
#include<stdio.h>
#include<GL/glut.h>

int xx[10]={0,210,285,360,210,285,360,210,285,360};
int yy[10]={0,260,260,260,185,185,185,110,110,110};
int a[10]={0,8,7,1,3,5,6,2,4,0};
int s1=0,s2=0,s3=0;
	
void display(void)
{
	

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	int flag=0,i;
    
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_POLYGON);
		glVertex2i(200,100);
		glVertex2i(200,325);
		glVertex2i(425,325);
		glVertex2i(425,100);
	glEnd();

	glBegin(GL_LINES);
		glVertex2i(427,327);
		glVertex2i(427,102);
		glVertex2i(426,326);
		glVertex2i(426,101);
	glEnd();

glColor3f(0,0,0.0);

	glBegin(GL_POINTS);
		glVertex2i(05,5);
//		glVertex2i(427,102);
//		glVertex2i(426,326);
//		glVertex2i(426,101);
	glEnd();

	glColor3f(1.0,0.0,0.0);

	glBegin(GL_LINE_LOOP);  // boundary lines 
		glVertex2i(200,100);
		glVertex2i(200,325);
		glVertex2i(425,325);
		glVertex2i(425,100);
	glEnd();



	glBegin(GL_LINES);  // for 3D effect 
		glVertex2i(427,327);
		glVertex2i(427,102);
		glVertex2i(426,326);
		glVertex2i(426,101);
        glVertex2i(428,328);
		glVertex2i(428,103);
		glVertex2i(201,326);
		glVertex2i(426,326);
		glVertex2i(202,327);
		glVertex2i(427,327);
        glVertex2i(203,328);
		glVertex2i(428,328);
	glEnd();
	

	glBegin(GL_LINES);  // horizontal and vertical lines
		glVertex2i(275,100);
		glVertex2i(275,325);
		glVertex2i(200,175);
		glVertex2i(425,175);
		glVertex2i(350,100);
		glVertex2i(350,325);
		glVertex2i(200,250);
		glVertex2i(425,250);
	glEnd();

	glColor3f(0.0,1.0,1.0);

	glRasterPos2i(240,50);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'M');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'O');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'V');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'E');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'S');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,':');

	glRasterPos2i(340,50);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s1+48);

	glRasterPos2i(328,50);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s2+48);

	glRasterPos2i(316,50);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s3+48);

	
for(int i=1;i<=9;i++)
{
	if(a[i]==0)
		continue;

	
	printf("\n %d %d ",xx[i],yy[i]);
		glColor3f(0.0,1.0,1.0);

    glBegin(GL_POLYGON);   
		glVertex2i(xx[i],yy[i]);
		glVertex2i(xx[i],yy[i]+55);
		glVertex2i(xx[i]+55,yy[i]+55);
		glVertex2i(xx[i]+55,yy[i]);
	glEnd();

    glColor3f(0.0,0.0,0.2);

	glBegin(GL_LINE_LOOP);  // boundary lines 
		glVertex2i(xx[i],yy[i]);
		glVertex2i(xx[i],yy[i]+55);
		glVertex2i(xx[i]+55,yy[i]+55);
		glVertex2i(xx[i]+55,yy[i]);
	glEnd();


	glColor3f(1.0,0.0,0.0);
	glRasterPos2i(xx[i]+23,yy[i]+23);
	int c=a[i]+48;
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,c);
  }
	
	for(i=1;i<=8;i++)
	{
		if(a[i] != i)
			flag=1;
	}
	if(flag==0)
	{	
			glRasterPos2i(300,400);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'W');
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'i');
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'n');
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'!');
	}
		

	glFlush();

}

int getblock(int x,int y)
{
		
			if(x>200 && x<275)
		{
				if(y>156 && y<231)
					return 1;
				
				if(y>231 && y<305)
					return 4;

				if(y>305 && y<380)
					return 7;
		}
		if(x>275 && x<350)
		{
				if(y>156 && y<231)
					return 2;
				
				if(y>231 && y<305)
					return 5;

				if(y>305 && y<380)
					return 8;
		}
		if(x>350 && x<425)
		{
				if(y>156 && y<231)
					return 3;
				
				if(y>231 && y<305)
					return 6;

				if(y>305 && y<380)
					return 9;
		}

		return 0;
}
void mouse(int btn,int state,int x,int y)
{
int loc,i,b;
if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) 
{
	loc=getblock(x,y);  
	printf("\n %d %d ",x,y);
	
	if(loc==0)
		return;

	for(i=1;i<=9;i++)
	{
		if(a[i]==0)
			b=i;          // holds blank location
	}

	if(loc==b)   // check selected loc is blank one 
		return;

	switch(b)
	{
	case 1: if(loc==2)
			{
				xx[b]=210;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==4)
			{
				xx[b]=210;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 9:
			if(loc==6)
			{
				xx[b]=360;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
				printf(" 666666666666666 ");
			}
			if(loc==8)
			{
				xx[b]=360;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}
			printf("\n Enter \n");
			break;
	case 2:
			
			if(loc==1)
			{
				xx[b]=285;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==3)
			{
				xx[b]=285;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==5)
			{
				xx[b]=285;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
		break;
	case 3:			
			if(loc==2)
			{
				xx[b]=360;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==6)
			{
				xx[b]=360;
				yy[b]=260;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 4:
			if(loc==1)
			{
				xx[b]=210;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==5)
			{
				xx[b]=210;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==7)
			{
				xx[b]=210;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 5:
			
			if(loc==2)
			{
				xx[b]=285;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==4)
			{
				xx[b]=285;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}

			if(loc==6)
			{
				xx[b]=285;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==8)
			{
				xx[b]=285;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 6:
			
			if(loc==3)
			{
				xx[b]=360;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==5)
			{
				xx[b]=360;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}	
			if(loc==9)
			{
				xx[b]=360;
				yy[b]=185;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 7:
			
		if(loc==4)
			{
				xx[b]=210;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}
		if(loc==8)
			{
				xx[b]=210;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	case 8:
			if(loc==5)
			{
				xx[b]=285;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}	

			if(loc==7)
			{
				xx[b]=285;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}
			if(loc==9)
			{
				xx[b]=285;
				yy[b]=110;
				a[b]=a[loc];
				a[loc]=0;
			}
			break;
	default: 
		printf("\n errorr ");
		break;
	}

	if(a[loc]==0)
	{
		s1=s1+1;
		if(s1==10)
		{
			s1=0;
			s2++;
		}
		if(s2==10)
		{
			s2=0;
			s3++;
		}
	}
	display();


//	glFlush();
	
   }
}
void myinit()
{
	glClearColor(1.0,1.0,0.5,0.0);
	glColor3f(0.0f,1.0f,1.0f);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);
}


int main(int argc,char ** argv)
{

		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
		glutInitWindowSize(640,480);
		glutCreateWindow(" GIGSAW PUZZLE ");
//		glutMouseFunc(mouse);
//		glutDisplayFunc(display);
		myinit();
		glutMainLoop();
		
}
/*
xx[7]=210;yy[7]=110;
	xx[8]=285;yy[8]=110;

	xx[3]=360;yy[3]=260;

	xx[4]=210;yy[4]=185;
	xx[5]=285;yy[5]=185;
	xx[6]=360;yy[6]=185;

	xx[1]=210;yy[1]=260;
	xx[2]=285;yy[2]=260;
*/
