#include <iostream>
#include <map>
#include "NativeGL.hxx"
#include <GL/glew.h>

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

class Buffer
{
  private:
    char* id;
  
    GLuint bufferId;
    int size;
    bool upToDate;
    bool generated;
    
    void checkGenerated()
    {
      if (!generated || !glIsBuffer(bufferId) )
      {
        std::cout << " GENERATE ";
        glGenBuffers(1, &bufferId);
        generated = true;
        upToDate = false;
      }
      
      if (glGetError() != GL_NO_ERROR)
      {
        std::cout << "Not able to generated buffer" << std::endl;
      }
    }

    void checkUpToDate()
    {    
      if (!upToDate)
      {
        std::cout << " UPDATE ";
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        
        double* t = (double*) getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector);
        int* size_p = (int*) getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int);
        size = *size_p;
        
        float* data = new float[size * 4];
        for (int i = 0 ; i < size ; i++)
        {
          data[4*i+0] = (float) t[i];
          data[4*i+1] = (float) t[i + size];
          data[4*i+2] = (float) t[i + 2*size]; 
          data[4*i+3] = 1.0f;
        }
                
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), data, GL_STATIC_DRAW);
        upToDate = true;
      } else {
        std::cout << " NO_UPDATE ";
      }
    }
    
  public:

    Buffer():
      id(NULL),
      upToDate(false),
      generated(false)
    {
      glewInit();
    }
  
    void bindBuffer(char* _id)
    {
      id = _id;
      std::cout << "bind : ";
      checkGenerated();
      checkUpToDate();
      std::cout << std::endl;
      
      glEnable(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, bufferId);
      glVertexPointer(4, GL_FLOAT, 0, 0);
      
      glDrawArrays(GL_LINE_STRIP, 0, size/4);
    }
    
    void needUpdate()
    {
      upToDate = false;
    }
};

static std::map<std::string, Buffer> bufferMap;

void loadGLData(char* id)
{
  bufferMap[id].bindBuffer(id);
}

void updateObject(char* id)
{
  std::map<std::string, Buffer>::iterator it = bufferMap.find(id);
  if (it != bufferMap.end())
  {
    it->second.needUpdate();
  }
}



