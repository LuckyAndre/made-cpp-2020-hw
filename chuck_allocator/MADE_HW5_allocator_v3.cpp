#include "iostream";
#include "vector";


using namespace std;

// Chunk - элемент фиксированного размера, группирую в блоки
struct Chunk {
    // когда не заполнен данными, то next укаывает на следующий элемент
    Chunk *next;
};


// блок чанков: управляет указателем на чанки, запрашивает создание новых блоков
template <typename T>
class Allocator {
public:
    using value_type = T; // через эту штуку можно вернуть тип который был при инициализации
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    Allocator(size_t chunksPerBlock): mChunksPerBlock(chunksPerBlock) {}

    // Аллокация чанков в блоке
    // Возвращает ссылку на первый чанк свободный в блоке
    void *Allocator::allocate(size_t size) {
        cout << "\nAllocating block (" << mChunksPerBlock << " chunks):\n\n";
        if (mAlloc == nullptr) {
            mAlloc = allocateBlock(size);
        }
        Chunk *freeChunk = mAlloc;
        mAlloc = mAlloc->next;
        return freeChunk;
    }

    // деаллокация
    void Allocator::deallocate(void *chunk, size_t size) {
        reinterpret_cast<Chunk *>(chunk)->next = mAlloc;
        mAlloc = reinterpret_cast<Chunk *>(chunk);
    }

private:
    size_t mChunksPerBlock;
    Chunk *mAlloc = nullptr;

    // Создание нового блока (кол-во чанков Х размер чанка)
    // После создания блока соединяет чанки указателем
    Chunk *Allocator::allocateBlock(size_t chunkSize) {
        size_t blockSize = mChunksPerBlock * chunkSize;
        // Первый кусок блока
        Chunk *blockBegin = reinterpret_cast<Chunk *>(malloc(blockSize));
        // Соединение чанков в блоке
        Chunk *chunk = blockBegin;
        for (int i = 0; i < mChunksPerBlock - 1; ++i) {
            chunk->next =
                    reinterpret_cast<Chunk *>(reinterpret_cast<char *>(chunk) + chunkSize);
            chunk = chunk->next;
        }
        chunk->next = nullptr;
        return blockBegin;
    }
};

int main() {
	return 0;
}

