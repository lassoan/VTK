/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOpenGLUniform
 * @brief   helper class to set custom uniform variables in GLSL shaders.
 *
 * This class implements all SetUniform* functions supported by vtkShaderProgram but instead of
 * directly calling the underlying OpenGL functions, it caches the name and value of the variable
 * and provides a mechanism for client mappers to set all cached variables at once in a generic way.
*/

#ifndef vtkOpenGLUniforms_h
#define vtkOpenGLUniforms_h

#include "vtkObject.h"
#include "vtkRenderingVolumeOpenGL2Module.h" // For export macro

#include <string> // For member functions

class vtkUniformInternals;
class vtkMatrix3x3;
class vtkMatrix4x4;
class vtkShaderProgram;

class VTKRENDERINGVOLUMEOPENGL2_EXPORT  vtkOpenGLUniforms : public vtkObject
{
public:
    static vtkOpenGLUniforms *New();
    vtkTypeMacro(vtkOpenGLUniforms, vtkObject);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    std::string GetDeclarations();
    bool SetUniforms( vtkShaderProgram * p );
    vtkMTimeType GetUniformListMTime();

    //@{
    /**
    * Position used to get or set the corner annotation text.
    * \sa GetText(), SetText()
    */
    enum UniformType
    {
      TypeInvalid = 0,
      Typei,             ///< integer
      Typef,             ///< float
      Type2i,            ///< pair of integers
      Type2f,            ///< pair of floats
      Type3f,            ///< triplet of floats
      Type3d,            ///< triplet of doubles
      Type4f,            ///< quadruplet of float
      Type3uc,           ///< triplet of unsigned char
      Type4uc,           ///< quadruplet of unsigned char
      TypeVtkMatrix3x3,  ///< vtkMatrix3x3
      TypeVtkMatrix4x4,  ///< vtkMatrix4x4
      TypeMatrix3x3,     ///< float matrix of size 3x3
      TypeMatrix4x4,     ///< float matrix of size 4x4
      Type1iv,           ///< integer vector of variable size
      Type1fv,           ///< float vector of variable size
      Type2fv,           ///< pair of floats vector of variable size
      Type3fv,           ///< triplet of floats vector of variable size
      Type4fv,           ///< quadruplet of floats vector of variable size
      TypeMatrix4x4v,    ///< matrix of 4x4 vector of variable size
      NumberOfUniformTypes // insert new types above this line
    };
    //@}

    /** Remove uniform variable named @p name */
    void RemoveUniform(const char *name);

    /** Remove all uniform variables */
    void RemoveAllUniforms();

    /** Set the @p name uniform value to @p v. */
    void SetUniformi(const char *name, int v);
    void SetUniformf(const char *name, float v);
    void SetUniform2i(const char *name, const int v[2]);
    void SetUniform2f(const char *name, const float v[2]);
    void SetUniform3f(const char *name, const float v[3]);
    void SetUniform3f(const char *name, const double v[3]);
    void SetUniform4f(const char *name, const float v[4]);
    void SetUniform3uc(const char *name, const unsigned char v[3]);
    void SetUniform4uc(const char *name, const unsigned char v[4]);
    void SetUniformMatrix(const char *name, vtkMatrix3x3 *v);
    void SetUniformMatrix(const char *name, vtkMatrix4x4 *v);
    void SetUniformMatrix3x3(const char *name, float *v);
    void SetUniformMatrix4x4(const char *name, float *v);

    /** Set the @p name uniform vector to @p f with @p count elements */
    void SetUniform1iv(const char *name, const int count, const int *f);
    void SetUniform1fv(const char *name, const int count, const float *f);
    void SetUniform2fv(const char *name, const int count, const float (*f)[2]);
    void SetUniform3fv(const char *name, const int count, const float (*f)[3]);
    void SetUniform4fv(const char *name, const int count, const float (*f)[4]);
    void SetUniformMatrix4x4v(const char *name, const int count, float *v);

    /** Get the @p name uniform value. Returns true on success. */
    bool GetUniformi(const char *name, int& v);
    bool GetUniformf(const char *name, float& v);
    bool GetUniform2i(const char *name, int v[2]);
    bool GetUniform2f(const char *name, float v[2]);
    bool GetUniform3f(const char *name, float v[3]);
    bool GetUniform3f(const char *name, double v[3]);
    bool GetUniform4f(const char *name, float v[4]);
    bool GetUniform3uc(const char *name, unsigned char v[3]);
    bool GetUniform4uc(const char *name, unsigned char v[4]);
    bool GetUniformMatrix(const char *name, vtkMatrix3x3 *v);
    bool GetUniformMatrix(const char *name, vtkMatrix4x4 *v);
    bool GetUniformMatrix3x3(const char *name, float *v);
    bool GetUniformMatrix4x4(const char *name, float *v);

    /** Get the @p name uniform vector to @p f with.
    Buffer must be pre-allocated based on vector size provided by
    */
    bool GetUniform1iv(const char *name, std::vector<int>& f);
    float* GetUniform1fv(const char *name);
    float* GetUniform2fv(const char *name);
    float* GetUniform3fv(const char *name);
    float* GetUniform4fv(const char *name);
    float* GetUniformMatrix4x4v(const char *name);

    /** Get number of all uniforms stored in this class */
    vtkIdType GetNumberOfUniforms();

    /** Get number of all uniforms stored in this class.
    Valid range is between 0 and GetNumberOfUniforms() - 1.*/
    const char* GetNthUniformName(vtkIdType uniformIndex);

    /** Get type of uniform @p name */
    UniformType GetUniformType(const char *name);

    /** Get length of a uniform @p name that contains a variable-size vector.
    Size includes number of tuples. For example, 3fv returns 3 x number of triplets. */
    vtkIdType GetUniformSize(const char *name);

protected:
  vtkOpenGLUniforms();
  ~vtkOpenGLUniforms() override;

private:
  vtkOpenGLUniforms(const vtkOpenGLUniforms&) = delete;
  void operator=(const vtkOpenGLUniforms&) = delete;

  vtkUniformInternals * Internals;
};

#endif
