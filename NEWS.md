## 0.1.1 ##
+ Bitmap font support to make it possible for renderer to draw text on provided textures

## 0.1.0 ##
First public commit.

The game engine heavily relies on the vision briefly discussed on page: https://kvk-tech.ru/post/devlog-cse-001
Currently Mini Serpent Engine has:
+ Logging system that sends messages to console window;
+ Platform system which consists of a simple 2D Renderer, Window management and events listener which all are bound to SDL;
+ Resource managemer system which doesn't plot allocated memory structure yet;
+ Window management system to provide access to windows created by the app in runtime;
+ Individual Layer manegement system per window;
+ Layers which are attached to windows by the engine user's design and can have graphical user interface (GUI) items which have their own editable callbacks;
+ GUI items (Canvas and a generic Button);
+ Application class itself.
