## Shop work principle

### UI
1. Player clicks on weapon slot
or
2. Player clicks on a trait slot that has a number attached to it

If Player clicked on a trait slot, its number is stored in `game.traitSlot`: 1 or 2 or 3.  
All characters have 2 slots, only Monk can possibly have 3 slots due to Monk doesn't wield weapons.  
Technically, Monk can wield Lightsaber, but that is not yet implemented, because Monk has no handles.  

So, ShopTraitUILayer can read game.traitSlot and get a clue on what trait is occupying the slot.  
When Player chooses trait, `game.playerCharacter` receives a trait in a corresponding slot.  

When Player chooses a weapon, `game.playerCharacter` receives it.

`ShopPageState` is used only to attach-detach layers.  
It has to know what layer to attach, so `game` has to store what shop to open.  
It is implemented via structures:
```cpp
namespace DTetris
{
	enum class ShopTypes
	{
		None        = 0,
		Weapons     = 1,
		Traits      = 2,
	};

	enum class TraitSlot
	{
		None    = 0,
		A       = 1,
		B       = 2,
		C       = 3,
	};
}
```
And variables inside `game`:
```cpp
TraitSlot traitSlot = TraitSlot::None; // for shop
ShopTypes shopType = ShopTypes::None; // for shop
```
