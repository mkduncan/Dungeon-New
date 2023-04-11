#include "Assets.hpp"
#include "Image.hpp"
#include "Logger.hpp"
#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>

Image::Image() : Mesh(), Texture(), Model(glm::mat4(1.0f)), Projection(glm::mat4(1.0f))
{
}

Image::~Image()
{
	if (!this->Destroy())
		Logger::LogError(__FUNCTION__, __LINE__);
}

Image::Image(const Image& image) : Mesh(image.Mesh), Texture(image.Texture), Model(image.Model), Projection(image.Projection)
{
}

Image::Image(Image&& image) noexcept : Mesh(std::move(image.Mesh)), Texture(std::move(image.Texture)), Model(image.Model), Projection(image.Projection)
{
	image.Model = glm::mat4(1.0f);
	image.Projection = glm::mat4(1.0f);
}

Image& Image::operator=(const Image& image)
{
	this->Mesh = image.Mesh;
	this->Texture = image.Texture;
	this->Model = image.Model;
	this->Projection = image.Projection;

	return (*this);
}

Image& Image::operator=(Image&& image) noexcept
{
	this->Mesh = std::move(image.Mesh);
	this->Texture = std::move(image.Texture);
	this->Model = image.Model;
	this->Projection = image.Projection;
	image.Model = glm::mat4(1.0f);
	image.Projection = glm::mat4(1.0f);

	return (*this);
}

const bool Image::Destroy()
{
	bool result = true;

	if (!this->Mesh.Destroy())
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		result = false;
	}

	if (!this->Texture.Destroy())
	{
		Logger::LogError(__FUNCTION__, __LINE__);
		result = false;
	}

	this->Model = glm::mat4(1.0f);
	this->Projection = glm::mat4(1.0f);

	return result;
}

const bool Image::Load(const std::string& imagePath)
{
	std::vector<VertexTextured> vertices;
	std::vector<GLuint> indices({ 0, 1, 2, 2, 3, 0 });

	if (!this->Destroy())
		return Logger::LogError(__FUNCTION__, __LINE__);

	vertices.push_back(VertexTextured(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec2(0.0f)));
	vertices.push_back(VertexTextured(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(VertexTextured(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec2(1.0f)));
	vertices.push_back(VertexTextured(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec2(0.0f, 1.0f)));

	if (!this->Mesh.Upload(vertices, indices))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!this->Texture.Load(imagePath))
	{
		this->Mesh.Destroy();
		return Logger::LogError(__FUNCTION__, __LINE__);
	}

	this->Model = glm::mat4(1.0f);
	this->Projection = glm::ortho(0.0f, Window::Width, Window::Height, 0.0f, -1.0f, 1.0f);

	return true;
}

const bool Image::Render() const
{
	if (!Assets::Shaders["UI"].Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!this->Texture.Enable())
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::Shaders["UI"].UploadMatrix("model", this->Model))
		return Logger::LogError(__FUNCTION__, __LINE__);

	else if (!Assets::Shaders["UI"].UploadMatrix("projection", this->Projection))
		return Logger::LogError(__FUNCTION__, __LINE__);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!this->Mesh.Render())
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		return Logger::LogErrorGL(__FUNCTION__, __LINE__);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return Logger::LogErrorGL(__FUNCTION__, __LINE__);
}

const bool Image::Position(const glm::vec2& position, const bool reset)
{
	if (reset)
		this->Model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

	else
		this->Model = this->Model * glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

	return true;
}

const bool Image::Position(const glm::vec2& position, const glm::vec2& referenceSize, const bool reset)
{
	glm::vec2 scaled = (position / referenceSize) * glm::vec2(Window::Width, Window::Height);

	if (reset)
		this->Model = glm::translate(glm::mat4(1.0f), glm::vec3(scaled, 0.0f));

	else
		this->Model = this->Model * glm::translate(glm::mat4(1.0f), glm::vec3(scaled, 0.0f));

	return true;
}

const bool Image::Rotate(const float angle, const bool reset)
{
	if (reset)
		this->Model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	else
		this->Model = this->Model * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	return true;
}

const bool Image::Scale(const glm::vec2& scale, const bool reset)
{
	if (reset)
		this->Model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

	else
		this->Model = this->Model * glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

	return true;
}

const bool Image::Scale(const glm::vec2& scale, const glm::vec2& referenceSize, const bool reset)
{
	glm::vec2 scaled = (scale / referenceSize) * glm::vec2(Window::Width, Window::Height);

	if (reset)
		this->Model = glm::scale(glm::mat4(1.0f), glm::vec3(scaled, 1.0f));

	else
		this->Model = this->Model * glm::scale(glm::mat4(1.0f), glm::vec3(scaled, 1.0f));

	return true;
}