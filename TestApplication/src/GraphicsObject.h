#pragma once

class GraphicsObject
{
protected:
	unsigned int _rendererId{};
public:
	virtual ~GraphicsObject() = default;
	virtual void Generate() = 0;
	virtual void Bind() const  = 0;
	virtual void Unbind() const = 0;
};