#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_GUITYPES_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_GUITYPES_H

#include <initializer_list>

namespace mse
{
	template <typename S>
	struct Symbol
	{
		S symbol;
		bool empty = true;
	};

	template <typename S, size_t size>
	struct SymbolsContainer
	{
		SymbolsContainer(std::initializer_list<S> list)
		{
			size_t position = 0;
			for (S element : list)
			{
				if (position < size)
				{
					input(element, position);
				} else {
					break;
				}
				++position;
			}
		}

		Symbol<S> container[size];

		void input(S symbol, size_t position = 0)
		{
			Symbol<S> temp = container[position];
			container[position].symbol = symbol;
			container[position].empty = false;

			if (!temp.empty)
			{
				for (size_t i = position; i < size; ++i)
				{
					temp = temp xor container[i];
					container[i] = temp xor container[i];
					temp = temp xor container[i];

					if (temp.empty)
					{
						break;
					}
				}
			}
		}

		// invoked by pressing "Delete" button
		void remove(S symbol, size_t position = 0)
		{
			if (position < size)
			{
				if (!container[position].empty)
				{
					container[position].empty = true;
					adjustSymbolsPositions();
				}
			}
		}

		// invoked by pressing "Backspace" button
		void backspace(S symbol, size_t position = 0)
		{
			if (position > 0)
			{
				if (position > size)
				{
					position = size-1;
				}

				if (!container[position].empty)
				{
					container[position].empty = true;
					adjustSymbolsPositions();
				}
			}
		}

		// access elements as if it's just an array
		S& operator[](size_t position)
		{
			if (position < size)
			{
				return container[position].symbol;
			}

			return container[size-1].symbol;
		}

		void adjustSymbolsPositions()
		{
			for (size_t i = 0; i < size; ++i)
			{
				if (container[i].empty)
				{
					for (size_t position = i, size_t j = position + 1; j < size; ++position, ++j)
					{
						if (!container[j].empty)
						{
							swap(position, j);
						} else {
							i = position;
							break;
						}
					}
				}
			}
		}

		void swap(size_t i, size_t j)
		{
			if (container[i].empty && !container[j].empty)
			{
				container[i].symbol = container[j].symbol;
				container[i].empty = container[j].empty;
				container[j].empty = true;
				return;
			}

			if (container[j].empty && !container[i].empty)
			{
				container[j].symbol = container[i].symbol;
				container[j].empty = container[i].empty;
				container[i].empty = true;
				return;
			}

			if (container[i].empty && container[j].empty)
			{
				return;
			}

			container[i].symbol = container[i].symbol xor container[j].symbol;
			container[j].symbol = container[i].symbol xor container[j].symbol;
			container[i].symbol = container[i].symbol xor container[j].symbol;

			container[i].empty = container[i].empty xor container[j].empty;
			container[j].empty = container[i].empty xor container[j].empty;
			container[i].empty = container[i].empty xor container[j].empty;
		}
	};
}

#endif
