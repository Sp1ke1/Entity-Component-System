# Entity Component System 

This project implements an Entity Component System (ECS) framework, a design pattern widely used in game development  and simulation systems. 

The ECS architecture provides a flexible and efficient way to manage entities, components, and systems, allowing for easy scalability and separation of concerns.

This implementation is built around a custom generic cache friendly PackedArray container, within which all Entity Component System classes are stored. 

The central class is [ECS](ECS/ECS.h). It contains the entire core API of the system. Most of the functions are duplicated in the format "FunctionName" "FunctionName"Checked. The latter provides a safe version of the former. The first variant is intended for pure speed. 