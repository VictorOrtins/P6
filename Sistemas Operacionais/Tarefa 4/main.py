import sys
import queue
import threading

def check_numeric_input(msg: str, exit_string: str) -> None:
    while True:
        try:
            numeric_input: str = input(f'{msg}: (Digite {exit_string} para sair) ')
            numeric_input: int = int(numeric_input)
            return numeric_input
        except ValueError:
            if numeric_input.lower() == exit_string.lower():
                sys.exit(-1)

def create_memory(num_frames_memory: int, frame_size: int) -> list:
    memory = []
    for i in range(num_frames_memory):
        memory.append([None])

    return memory

def get_logical_address() -> dict:
    page: int = check_numeric_input('Qual a página a ser acessada', 'N')
    offset: int = check_numeric_input('Qual o deslocamento', 'N')

    return {'page': page, 'offset':offset}

def get_memory_access_sequence(num_memory_access: int) -> list:
    memory_access_sequence: list = []

    for i in range(num_memory_access):
        print(f'\nAcesso número {i+1}: ')
        logical_address: dict = get_logical_address()
        memory_access_sequence.append(logical_address)

    return memory_access_sequence

def get_page_table(memory_access_sequence: list) -> dict:
    page_table: dict = {}

    for logical_address in memory_access_sequence:
        if logical_address['page'] not in page_table:
            page_table[logical_address['page']] = {'frame': -1, 'in_memory': False}
    
    return page_table

def add_page_to_memory(page: int, page_table: dict, max_frames_memory: int, fifo: queue.Queue):

    num_frames_memory = 0
    index_frames_memory = []
    for _, page_info in page_table.items():
        if page_info['in_memory']:
            num_frames_memory += 1
            index_frames_memory.append(page_info['frame'])

    if num_frames_memory == 0:
        page_table[page]['frame'] = 0
        page_table[page]['in_memory'] = True
        fifo.put(page)
        return
    
    if num_frames_memory < max_frames_memory:
        index_frames_memory.sort()

        contador = 0
        for i in range(max_frames_memory):
            if i >= len(index_frames_memory):
                break

            if i != index_frames_memory[i]:

                page_table[page]['frame'] = i
                page_table[page]['in_memory'] = True
                fifo.put(page)
                return

            contador += 1

            
        page_table[page]['frame'] = contador
        page_table[page]['in_memory'] = True
        fifo.put(page)
        return
            
    if fifo.full():
        page_removed = fifo.get()
        empty_frame = page_table[page_removed]['frame'] 
        page_table[page_removed]['in_memory'] = False

        page_table[page]['frame'] = empty_frame
        page_table[page]['in_memory'] = True
        fifo.put(page)
        return
    else:
        print('!!!!!!!!!!!!!!!!!!!!! --- ERRO --- !!!!!!!!!!!!!!!!!!!!!!!!!')
        sys.exit(-10)


def print_table(page_table: dict):
    print('|   Page   |   Frame   |   In Memory   |')
    for page, page_info in page_table.items():
        frame = page_info['frame']
        in_memory = page_info['in_memory']
        print(f'|     {page}     |      {frame}     |     {in_memory}     |')

def print_memory(page_table: dict, max_frames_memory: int):
    print('|   Memory   |')
    for i in range(max_frames_memory):
        page_in_frame = False
        for page, page_info in page_table.items():
            if i == page_info['frame'] and page_info['in_memory']:
                print(f'|     {page}     |')
                page_in_frame = True
        
        if not page_in_frame:
            print(f'|           |')


def main():
    print('--- Escolha a configuração de Quadros desejada ---')

    max_frames_memory: int = check_numeric_input('Quantidade máxima de Quadros na memória', 'Sair')
    frame_size: int = check_numeric_input('Tamanho dos Quadros', 'Sair')
    print()


    num_memory_access: int = check_numeric_input('Quantidade de Acessos que serão feitos na memória', 'Sair')
    memory_access_sequence: list = get_memory_access_sequence(num_memory_access)

    page_table: dict = get_page_table(memory_access_sequence)
    
    fifo = queue.Queue(maxsize=max_frames_memory)

    for logical_address in memory_access_sequence:
        page, offset = logical_address['page'], logical_address['offset']
        
        print(f'--- Endereço lógico requisitado: (Página: {page}, Deslocamento: {offset}) --- ')

        if logical_address['offset'] >= frame_size:
            print('----- Segmentation Fault!!! -----')
            continue

        if not page_table[logical_address['page']]['in_memory']:
            add_page_to_memory(logical_address['page'], page_table, max_frames_memory, fifo)

        print_table(page_table)
        print()
        print_memory(page_table, max_frames_memory)

if __name__ == '__main__':
    main()





