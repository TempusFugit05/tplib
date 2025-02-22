#ifndef BASE_TYPES_H
#define BASE_TYPES_H

namespace tp
{
    struct array_t
    {
        private:
            size_t size;
        
        public:
            void* buffer;
            
        public
            size_t get_size()
            {
                return size;
            }

            array_t(size_t size, void* buffer) :
            size(size), buffer(buffer)
            {}
    };
}

#endif // BASE_TYPES_H
