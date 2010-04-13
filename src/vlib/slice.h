#ifndef __SLICE_H__
#define __SLICE_H__

template <typename T>
class slice
{
	public:
		slice()	:
			length(0),
			first(NULL)
			{}

		slice(void* data) :
			length(0),
			first((T*)data)
			{}

		slice(void* data, int s) :
			length(s),
			first((T*)data)
			{}

		// Get and Set the length of the slice
		int		Length() const		{ return length; }
		void	SetLength(int s)	{ length = s; }

		// Change the memory pointed to by this slice
		void Relocate(void* data) {
			T* dest = (T*)data;
			repeat(length) {
				dest[i] = first[i];	
			}
			first = dest;	// Don't implicitly free the memory, slices do not `own` their memory
		}

		// Add a new item to the end of the slice
		void Append(T data)		{
			first[length] = data;
			++length;
		}

		void CopyAll(slice<T>& source) {
			repeat(source.Length())	{
				Append(source[i]);	
			} }

		// Reset the slice to zero, to be reused safely
		void Reset() {
			length = 0;
			first = NULL;
		}

		// Array access operator
		T&			operator [] (int i)			{ return first[i]; }
		const T&	operator [] (int i) const	{ return first[i]; }

	protected:
		int	length;
		T*	first;
};

#endif // __SLICE_H__
