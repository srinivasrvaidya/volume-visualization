//#define GLM_FORCE_RADIANS

#include <fstream>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>


#define GL_ERROR() checkForOpenGLError(__FILE__, __LINE__)

using namespace std;
using namespace cv;

using glm::mat4;
using glm::vec3;
GLuint g_vertexArrayObject;
GLuint g_vho;
GLuint g_programHandle;
GLuint g_winWidth ;
GLuint g_winHeight ;
GLint  g_angle = 0;
GLuint g_frameBuffer;
GLuint g_tffTexObj;
GLuint g_bfTexObj;
GLuint g_texWidth;
GLuint g_texHeight;
GLuint g_volTexObj;
GLuint g_rcVertHandle;
GLuint g_rcFragHandle;
GLuint g_bfVertHandle;
GLuint g_bfFragHandle;
GLuint g_histogramVertHandle;
GLuint g_histogramFragHandle;
GLuint g_TestFrameBuffer;
GLuint g_histogramTexObj;
GLuint g_3DVolHeight;
GLuint g_3DVolBreath;
GLuint g_3DVolWidth;


float sampleOpacity[50] = {0.0};
float g_stepSize;
bool rotationFlag;
float ROI[6] = {0.6, 0.6, 0.6, 0.4, 0.4, 0.4};
char *volumeFile;

/** function declaration **/

void keyboard(unsigned char key, int x, int y);
void display(void);
void initVertexBufferObject();
void initShader();
void initFrameBuffer(GLuint, GLuint, GLuint);
GLuint initTFF1DTex(const char* filename);
GLuint initFace2DTex(GLuint texWidth, GLuint texHeight);
GLuint initVolume3DTexture(const char* filename, GLuint width, GLuint height, GLuint depth);
void initTestFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight);
void render(GLenum cullFace);

int checkForOpenGLError(const char* file, int line)
// return 1 if an OpenGL error occured, 0 otherwise.
{
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
		cout << "glError in file " << file
			 << "@line " << line << gluErrorString(glErr) << endl;
			retCode = 1;
		exit(EXIT_FAILURE);
    }
    return retCode;
}

void initInitializeGlobalData()
{

    rotationFlag = true;

    g_stepSize = 0.001f;
    g_winWidth = 500;
    g_winHeight = 500;

    /*** Transfer function  *****/

    // { noOfItemsPassed, RangeUpperLimit, RangeLowerLimit, R, B, G, A , ...}
    int TFLen = 31;
    float transferFunction[31] = {TFLen,    1.00, 0.35, 1.00, 0.00, 0.00, 0.01,
                                            0.35, 0.18, 0.0, 1.0, 0.0, 0.01,
                                            0.18, 0.08, 0.0, 0.0, 1.0, 0.004,
                                            0.08, 0.05, 0.05, 0.50, 0.20, 0.01 };


    for(int i =0; i<TFLen; i++)                                     
    {
        sampleOpacity[i] = transferFunction[i];        
    }


    /**** 3D data controls ********/

    g_3DVolHeight = 256;
    g_3DVolWidth = 256;
    g_3DVolBreath = 256;

    volumeFile = "./model/foot.raw";
     // "./model/bonsai.raw" 
     // "./model/skull.raw"
     // "./model/BostonTeapot.raw"   -- 256 256 178
     // "./model/foot.raw"
}


void init()
{


    g_texWidth = g_winWidth;
    g_texHeight = g_winHeight;
    initVertexBufferObject();
    initShader();
    g_tffTexObj = initTFF1DTex("tff.dat");
    g_bfTexObj = initFace2DTex(g_texWidth, g_texHeight);
    g_histogramTexObj = initFace2DTex(g_texWidth, g_texHeight);

    g_volTexObj = initVolume3DTexture( volumeFile, g_3DVolHeight, g_3DVolBreath, g_3DVolWidth);

    GL_ERROR();
    initFrameBuffer(g_bfTexObj, g_texWidth, g_texHeight);
    initTestFrameBuffer(g_histogramTexObj, g_texWidth, g_texHeight);
    GL_ERROR();
    
    initInitializeGlobalData();

}

void initVertexBufferObject()
{
    GLfloat vertices[24] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
    };
			
    GLuint indices[36] = {
	1,5,7,
	7,3,1,
	0,2,6,
    6,4,0,
	0,1,3,
	3,2,0,
	7,5,4,
	4,6,7,
	2,3,7,
	7,6,2,
	1,0,4,
	4,5,1
    };
    GLuint gbo[2];
    
    glGenBuffers(2, gbo);
    GLuint vertexdat = gbo[0];
    GLuint veridxdat = gbo[1];
    glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

    // used in glDrawElement()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    // vao like a closure binding 3 buffer object: verlocdat vercoldat and veridxdat
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0); // for vertexloc
    glEnableVertexAttribArray(1); // for vertexcol

    // the vertex location is the same as the vertex color
    glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
    glBindVertexArray(0);
    g_vertexArrayObject = vao;
    
}
void drawBox(GLenum glFaces)
{
    glEnable(GL_CULL_FACE);
    glCullFace(glFaces);
    glBindVertexArray(g_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
    glDisable(GL_CULL_FACE);
}


GLboolean compileCheck(GLuint shader)
// Check the compilation result
{
    GLint err;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (GL_FALSE == err)
    {
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);
			cout << " Shader log: " << log << endl;
			free(log);		
		}
    }
    return err;
}


GLuint initShaderObj(const GLchar* srcfile, GLenum shaderType)
// init shader object
{
    ifstream inFile(srcfile, ifstream::in);
    // use assert?
    if (!inFile)
    {
		cout << "Error openning file: " << srcfile << endl;
		exit(EXIT_FAILURE);
    }
    
    const int MAX_CNT = 10000;
    GLchar *shaderCode = (GLchar *) calloc(MAX_CNT, sizeof(GLchar));
    inFile.read(shaderCode, MAX_CNT);
    if (inFile.eof())
    {
		size_t bytecnt = inFile.gcount();
		*(shaderCode + bytecnt) = '\0';
    }
    else if(inFile.fail())
    {
		cout << srcfile << "read failed " << endl;
    }
    else
    {
		cout << srcfile << "is too large" << endl;
    }
    
    // create the shader Object
    GLuint shader = glCreateShader(shaderType);
    if (0 == shader)
    {
		cout << "Error creating vertex shader." << endl;
    }

    const GLchar* codeArray[] = {shaderCode};
    glShaderSource(shader, 1, codeArray, NULL);
    free(shaderCode);

    // compile the shader
    glCompileShader(shader);
    if (GL_FALSE == compileCheck(shader))
    {
		cout << "shader compilation failed" << endl;
    }
    return shader;
}


GLint checkShaderLinkStatus(GLuint pgmHandle)
{
    GLint status;
    glGetProgramiv(pgmHandle, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
    {
		GLint logLen;
		glGetProgramiv(pgmHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			GLchar * log = (GLchar *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(pgmHandle, logLen, &written, log);
			cout << "Program log: " << log << endl;
		}
    }
    return status;
}

// link shader program
GLuint createShaderPgm()
{
    // Create the shader program
    GLuint programHandle = glCreateProgram();
    if (0 == programHandle)
    {
		cout << "Error create shader program" << endl;
		exit(EXIT_FAILURE);
    }
    return programHandle;
}


// init the 1 dimentional texture for transfer function
GLuint initTFF1DTex(const char* filename)
{
    // read in the user defined data of transfer function
    ifstream inFile(filename, ifstream::in);
    
    if (!inFile)
    {
		cout << "Error openning file: " << filename << endl;
		exit(EXIT_FAILURE);
    }
    
    const int MAX_CNT = 10000;
    GLubyte *tff = (GLubyte *) calloc(MAX_CNT, sizeof(GLubyte));
    inFile.read(reinterpret_cast<char *>(tff), MAX_CNT);

    if (inFile.eof())
    {
		size_t bytecnt = inFile.gcount();
		*(tff + bytecnt) = '\0';
		cout << "bytecnt " << bytecnt << endl;
    }
    else if(inFile.fail())
    {
		cout << filename << "read failed " << endl;
    }
    else
    {
		cout << filename << "is too large" << endl;
    }   
    
    cout << "\n " << *tff ;
    
    
     
    GLuint tff1DTex;
    glGenTextures(1, &tff1DTex);
    glBindTexture(GL_TEXTURE_1D, tff1DTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);
    free(tff);    
    return tff1DTex;
}

GLuint initFace2DTex(GLuint backfaceTextureWidth, GLuint backfaceTextureHeight)
// init the 2D texture for render backface
{
    GLuint backFace2DTex;
    glGenTextures(1, &backFace2DTex);
    glBindTexture(GL_TEXTURE_2D, backFace2DTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, backfaceTextureWidth, backfaceTextureHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    return backFace2DTex;
}


GLuint initVolume3DTexture(const char* filename, GLuint w, GLuint h, GLuint d)
// init 3D texture to store the volume data used fo ray casting
{
    
    FILE *fp;
    size_t size = w * h * d;
    GLubyte *data = new GLubyte[size];			  // 8bit
    if (!(fp = fopen(filename, "rb")))
    {
        cout << "Error: opening .raw file failed" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "OK: open .raw file successed" << endl;
    }
    if ( fread(data, sizeof(char), size, fp)!= size) 
    {
        cout << "Error: read .raw file failed" << endl;
        exit(1);
    }
    else
    {
        cout << "OK: read .raw file successed" << endl;
    }
    fclose(fp);

    glGenTextures(1, &g_volTexObj);
    // bind 3D texture target
    glBindTexture(GL_TEXTURE_3D, g_volTexObj);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
 
    // pixel transfer happens here from client to OpenGL server
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,data);

    delete []data;
    cout << "volume texture created" << endl;
    return g_volTexObj;
}

void checkFramebufferStatus()
{
    GLenum complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (complete != GL_FRAMEBUFFER_COMPLETE)
    {
		cout << "framebuffer is not complete" << endl;
		exit(EXIT_FAILURE);
    }
}

// init the framebuffer, the only framebuffer used in this program
void initFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight)
{
	
    // create a depth buffer for our framebuffer
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

    // attach the texture and the depth buffer to the framebuffer
    glGenFramebuffers(1, &g_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    checkFramebufferStatus();
    glEnable(GL_DEPTH_TEST);    
}    
    
void initTestFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight)    
{
/*  
	 // create a depth buffer for our framebuffer
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);
    
*/    
	    
    glGenFramebuffers(1, &g_TestFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_TestFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);

 //   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
 
    checkFramebufferStatus();
    glEnable(GL_DEPTH_TEST);    
   
}

void rayCastSetUnifroms()
{
   
    GLint screenSizeLoc = glGetUniformLocation(g_programHandle, "ScreenSize");
    if (screenSizeLoc >= 0)
    {
		glUniform2f(screenSizeLoc, (float)g_winWidth, (float)g_winHeight);
    }
    else
    {
		cout << "ScreenSize" << "is not bind to the uniform" << endl;
    }
    
    
    GLint stepSizeLoc = glGetUniformLocation(g_programHandle, "StepSize");
    GL_ERROR();
    if (stepSizeLoc >= 0)
    {
		glUniform1f(stepSizeLoc, g_stepSize);
    }
    else
    {
		cout << "StepSize" << "is not bind to the uniform "<< endl;
    }
    
    GL_ERROR();
    GLint transferFuncLoc = glGetUniformLocation(g_programHandle, "TransferFunc");
    if (transferFuncLoc >= 0)
    {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, g_tffTexObj);
		glUniform1i(transferFuncLoc, 0);
    }
    else
    {
	//	cout << "TransferFunc" << "is not bind to the uniform" << endl;
    }
     
    GL_ERROR();    
    GLint backFaceLoc = glGetUniformLocation(g_programHandle, "ExitPoints");
    if (backFaceLoc >= 0)
    {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_bfTexObj);
		glUniform1i(backFaceLoc, 1);
    }
    else
    {
		cout << "ExitPoints"
			 << "is not bind to the uniform"
			 << endl;
    }
    GL_ERROR();    


    GLint volumeLoc = glGetUniformLocation(g_programHandle, "VolumeTex");
    if (volumeLoc >= 0)
    {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, g_volTexObj);
		glUniform1i(volumeLoc, 2);
    }
    else
    {
		cout << "VolumeTex is not bind to the uniform" << endl;
    }
	

    GLint sampleOpacityLoc = glGetUniformLocation(g_programHandle, "sampleOpacity");
    if (sampleOpacityLoc >= 0)
    {
		glUniform1fv(sampleOpacityLoc, 50, sampleOpacity);
    }
    else
    {
			cout << "sampleOpacityLoc is not bind to the uniform" << endl;
    }
    

    GLint ROILoc = glGetUniformLocation(g_programHandle, "ROI");
    if ( ROILoc >= 0)
    {
        glUniform1fv( ROILoc, 6, ROI);
    }
    else
    {
            cout << "ROILoc is not bind to the uniform" << endl;
    }

}


// init the shader object and shader program
void initShader()
{
	

    g_bfVertHandle = initShaderObj("shader/backface.vert", GL_VERTEX_SHADER);

    g_bfFragHandle = initShaderObj("shader/backface.frag", GL_FRAGMENT_SHADER);

    g_rcVertHandle = initShaderObj("shader/raycasting.vert", GL_VERTEX_SHADER);

    g_rcFragHandle = initShaderObj("shader/raycasting.frag", GL_FRAGMENT_SHADER);

    g_histogramVertHandle = initShaderObj("shader/histogramRC.vert", GL_VERTEX_SHADER);

    g_histogramFragHandle = initShaderObj("shader/histogramRC.frag", GL_FRAGMENT_SHADER);

    g_programHandle = createShaderPgm();

}

// link the shader objects using the shader program
void linkShader(GLuint shaderPgm, GLuint newVertHandle, GLuint newFragHandle)
{
	
    const GLsizei maxCount = 2;
    GLsizei count;
    GLuint shaders[maxCount];
    glGetAttachedShaders(shaderPgm, maxCount, &count, shaders);
    
    // cout << "get VertHandle: " << shaders[0] << endl;
    // cout << "get FragHandle: " << shaders[1] << endl;
    
    GL_ERROR();
    for (int i = 0; i < count; i++) 
    {
		glDetachShader(shaderPgm, shaders[i]);
    }
    
    // Bind index 0 to the shader input variable "VerPos"
    glBindAttribLocation(shaderPgm, 0, "VerPos");
    
    // Bind index 1 to the shader input variable "VerClr"
    glBindAttribLocation(shaderPgm, 1, "VerClr");
    
  
   
    GL_ERROR();
    
    glAttachShader(shaderPgm,newVertHandle);
    glAttachShader(shaderPgm,newFragHandle);
   
    GL_ERROR();
   
    glLinkProgram(shaderPgm);
    if (GL_FALSE == checkShaderLinkStatus(shaderPgm))
    {
		cout << "Failed to relink shader program!" << endl;
		exit(EXIT_FAILURE);
    }
    GL_ERROR();
}

void readBufferTexture(GLuint g_TestFrameBuffer)
{

  

	int size = g_winHeight * g_winWidth * 4;
	float texValue[size] ;
    GLuint i=0;
    int hist_w = 500; int hist_h = 500;
	float rangeTotal[4] = {0.0,0.0,0.0,0.0};
    float total=0.0;
		
	glBindFramebuffer(GL_FRAMEBUFFER, g_TestFrameBuffer);
	glEnable(GL_TEXTURE_2D);
	glReadPixels(0, 0, g_winWidth, g_winHeight, GL_RGBA, GL_FLOAT, texValue);

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	
	for(i = 0 ; i < (g_winWidth * g_winHeight) ; i++)
	{
		rangeTotal[0] += texValue[4*i];
		rangeTotal[1] += texValue[4*i+1];	
		rangeTotal[2] += texValue[4*i+2];
		rangeTotal[3] += texValue[4*i+3];

//        if( texValue[4*i] > 0)
//            cout << " Values:: " << texValue[4*i] << " " << texValue[4*i+1] << " " << texValue[4*i+2] << " " << texValue[4*i+3] << "\n";
	}
	

    i=0;
    while(i<4)
        total += rangeTotal[i++];

    i=0;
    while(i < 4)
    {
        if ( rangeTotal[i] < 1.0 )
        {
            rangeTotal[i] = 0.00001;
            i++;
            continue;            
        }    
        rangeTotal[i] /= total;
        i++;
    }       


    cout << " Values:: " << rangeTotal[0]*100 << " " << rangeTotal[1]*100 << " " << rangeTotal[2]*100 << " " << rangeTotal[3]*100 << "\n";

    rangeTotal[0] *= 500;
    rangeTotal[1] *= 500;
    rangeTotal[2] *= 500;
    rangeTotal[3] *= 500;

 //   cout << " Values:: " << rangeTotal[0] << " " << rangeTotal[1] << " " << rangeTotal[2] << " " << rangeTotal[3] << "\n";

    line( histImage, Point(100, 500) , Point(100, 550 - rangeTotal[0]), Scalar( 255, 0, 0), 12, 8, 0  );
    line( histImage, Point(200, 500) , Point(200, 550 - rangeTotal[1]), Scalar( 0, 255, 0), 12, 8, 0  );
    line( histImage, Point(300, 500) , Point(300, 550 - rangeTotal[2]), Scalar( 0, 0, 255), 12, 8, 0  );
    line( histImage, Point(400, 500) , Point(400, 550 - rangeTotal[3]), Scalar( 150, 150, 150), 12, 8, 0  );

    namedWindow("Visibility histogram", CV_WINDOW_AUTOSIZE );
    imshow("Visibility histogram", histImage );
    
    waitKey(1);
    histImage.setTo(Scalar(0,0,0));
    

}


void display()
{
   
  //  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
    GL_ERROR();
    
    // render to texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_frameBuffer);
    glViewport(0, 0, g_winWidth, g_winHeight);
    linkShader(g_programHandle, g_bfVertHandle, g_bfFragHandle);
    glUseProgram(g_programHandle);
    render(GL_FRONT);
    glUseProgram(0);
    GL_ERROR();
    
    /* -- histogram -- */


    glBindFramebuffer(GL_FRAMEBUFFER, g_TestFrameBuffer);
    glViewport(0, 0, g_winWidth, g_winHeight);
    linkShader(g_programHandle, g_histogramVertHandle, g_histogramFragHandle);
    GL_ERROR();
    glUseProgram(g_programHandle);
    rayCastSetUnifroms();
    GL_ERROR();
    render(GL_BACK);
    glUseProgram(0);
    GL_ERROR(); 
    
    readBufferTexture(g_TestFrameBuffer);   
    
    /* -- Ray cast to frame buffer to display-- */
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, g_winWidth, g_winHeight);
    linkShader(g_programHandle, g_rcVertHandle, g_rcFragHandle);
    GL_ERROR();
    glUseProgram(g_programHandle);
    rayCastSetUnifroms();
    GL_ERROR();
    // glUseProgram(g_programHandle);
    // cull back face
    render(GL_BACK);
    glUseProgram(0);
    GL_ERROR(); 
    
    // for test the first pass
/*
     glBindFramebuffer(GL_READ_FRAMEBUFFER, g_frameBuffer);
     checkFramebufferStatus();
     glViewport(0, 0, g_winWidth, g_winHeight);
     glClearColor(0.0, 0.0, 1.0, 1.0);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     GL_ERROR();
     glBlitFramebuffer(0, 0, g_winWidth, g_winHeight,0, 0, g_winWidth, g_winHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     GL_ERROR();
     */
      
    glutSwapBuffers();
    
}

void render(GLenum cullFace)
{
    GL_ERROR();
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  transform the box
    glm::mat4 projection = glm::perspective(60.0f, (GLfloat)g_winWidth/g_winHeight, 0.1f, 400.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f),
    				 glm::vec3(0.0f, 0.0f, 0.0f), 
    				 glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = mat4(1.0f);
    model *= glm::rotate((float)g_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	// to make the "*.raw" i.e. the volume data stand up.
 
//	model *= glm::rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
    model *= glm::rotate(90.0f, vec3(0.0f, 1.0f, 0.0f));

    model *= glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f)); 
    
    // Multiplication order: reverse order of transform
    glm::mat4 mvp = projection * view * model;
  
    GLuint mvpIdx = glGetUniformLocation(g_programHandle, "MVP");
    if (mvpIdx >= 0)
    {
    	glUniformMatrix4fv(mvpIdx, 1, GL_FALSE, &mvp[0][0]);
    }
    else
    {
    	cout << "can't get the MVP" << endl;
    }
    
    GL_ERROR();
	
	drawBox(cullFace);
    
    GL_ERROR();
  
  //glutWireTeapot(0.5);
}
void rotateDisplay()
{
   
    if ( rotationFlag )
        g_angle = (g_angle + 1) % 360;
    
    glutPostRedisplay();
}
void reshape(int w, int h)
{
    g_winWidth = w;
    g_winHeight = h;
    g_texWidth = w;
    g_texHeight = h;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
	case 'A':
	case 'a':	
                    if ( sampleOpacity[6] < 0.005)
                        sampleOpacity[6] = 0.0000 ;       
                    else    
                       sampleOpacity[6] -= 0.005;
	

    break;
	
	case 'D':
	case 'd':
				sampleOpacity[6] +=0.005;
	break;
    case 'q':
    case 'Q':   
                    if ( sampleOpacity[12] < 0.005)
                        sampleOpacity[12] = 0.0000 ;      
                    else    
                       sampleOpacity[12] -= 0.005;
    break;
    
    case 'e':
    case 'E':
                sampleOpacity[12] +=0.005;
    break;

    case 'L':    // move left
    case 'l':
                ROI[2] -= 0.05;
                ROI[5] -= 0.05;
    break;

    case 'j':    // move right
    case 'J':
                ROI[2] += 0.05;
                ROI[5] += 0.05;
    break;


    case 'M': // move front
    case 'm':
                ROI[0] -= 0.05;
                ROI[3] -= 0.05;
    break;


    case 'n': // move back
    case 'N':
                ROI[0] += 0.05;
                ROI[3] += 0.05;
    break;

    case 'i': // move front
    case 'I':
                ROI[1] += 0.05;
                ROI[4] += 0.05;
    break;


    case 'K': // move back
    case 'k':
                ROI[1] -= 0.05;
                ROI[4] -= 0.05;
    break;

    case '\x20' :
            rotationFlag = !rotationFlag;
            break;
    case '\x1B':
	
				exit(EXIT_SUCCESS);

	break;
	
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    
    initInitializeGlobalData();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_winWidth, g_winWidth);
    glutCreateWindow(" RayCasting Algorithm");
    GLenum err = glewInit();
    
    if (GLEW_OK != err)
    {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
  
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&display);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&rotateDisplay);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}


/* draw the six faces of the boundbox by drawing triangles
   draw it contra-clockwise
   // front: 1 5 7 3
   // back: 0 2 6 4
   // left£º0 1 3 2
   // right:7 5 4 6    
   // up: 2 3 7 6
   // down: 1 0 4 5 */
