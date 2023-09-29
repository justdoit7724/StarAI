#pragma once

template<typename T>
class SwapChain {
public:

	SwapChain(T d1, T d2) {
		data1 = std::move(d1);
		data2 = std::move(d2);

		getPtr = &data1;
		workPtr = &data2;
	}

	void Swap() {
		if (getPtr == &data1)
		{
			getPtr = &data2;
			workPtr = &data1;
		}
		else
		{
			getPtr = &data1;
			workPtr = &data2;
		}
	}

	const T& Get() const { return *getPtr; }
	T* Work() { return workPtr; }

private:
	T* getPtr;
	T* workPtr;
	T data1;
	T data2;
};