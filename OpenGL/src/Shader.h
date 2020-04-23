#pragma 
#include <string>
class Shader {
private:
	unsigned int m_RenderID;
public:
	Shader(const std::string& vertexFilename, const std::string& fragmentFilename);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);

};