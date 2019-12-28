#pragma once

class GraphicsObject
{
protected:
	unsigned int _rendererId{};
public:
	virtual ~GraphicsObject() = default;
	virtual void Bind() const  = 0;
	virtual void Unbind() const = 0;
};