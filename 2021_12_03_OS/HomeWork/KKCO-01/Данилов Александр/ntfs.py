#!/usr/bin/python3
import sys
from struct import unpack

if len(sys.argv) != 2:
    print(f"Use: {sys.argv[0]} image_name")
    exit(1)
# Проверка возможности открыть фаил
try:
    fd = open(sys.argv[1],"rb") 
    fd.close()  
except:
    print(f"Error open {sys.argv[1]}")
    exit(1)

with open(sys.argv[1], "rb") as fd:
    # Читаем boot
    data = fd.read(512)
    if not data: 
        print("Error reading of boot")
        exit(1)
    
    size_sector, size_clust, address_MFT, address_MFTMirr, size_entry, signature = unpack("=11xHB34xQQb445xH", data)
    
    if size_entry == -10:
        size_entry = 1024
    else:
        print("Error size of entry")
        exit(1)

    if signature != 0xAA55:
        print("Error of signature")
        exit(1)
    
    print(f"""Boot:
    Размер сектора: {size_sector} байт
    Размер кластера: {size_clust} секторов
    Размер записи MFT: {size_entry} байт
    Адрес MFT: {address_MFT} кластер
    Адрес MFTMirr: {address_MFTMirr} кластер
    """)
    
    # Смещаемся до MFT
    fd.seek(address_MFT*size_clust*size_sector, 0)

    # * Первая итерация - $MFT
    while True:
        # чтение записи
        data = fd.read(size_entry)
        # определяем тип записи
        type_entry = unpack("=I", data[:4])
        if type_entry[0] == 0x454c4946:
            type_entry = "FILE"
        elif type_entry[0] == 0x58444e49:
            type_entry = "INDX"
        elif type_entry[0] == 0x454c4f48:
            type_entry = "HOLE"
        elif type_entry[0] == 0x444b4843:
            type_entry = "CHKD"
        elif type_entry[0] == 0x44414142:
            type_entry = "BAAD"
        else: 
            exit(0)
        # разбор структуры записи
        offset_arr_markers, size_arr_markers, index_LSN, sequence_number, count_links, offset_atr, flag, real_size_entry, allocated_size_entry, address_base_entry, index_new_atr, index_entry= unpack("=4xHHQHHHHIIQH2xI",data[:48] ) 
        print(f"""Номер записи: {index_entry}
        Тип записи: {type_entry}
        Смещение массива маркеров: {offset_arr_markers}
        Количество элементов в массиве маркеров: {size_arr_markers} 
        Номер записи в журнале транзакций: {index_LSN}
        Порядковый номер: {sequence_number}
        Счетчик ссылок: {count_links}
        Смещение первого атрибута: {offset_atr}
        Флаг: {flag}
        Используемый размер записи MFT: {real_size_entry}
        Выделенный размер записи MFT: {allocated_size_entry}
        Адрес базовой записи MFT: {address_base_entry}
        Идентификатор для нового атрибута: {index_new_atr}
        Номер файловой записи: {index_entry}
        """)
        offset = offset_atr
        atr = unpack("=I", data[offset:offset+4])
        # Разбор атрибутов
        while(atr[0] != 0xFFFFFFFF):
            size_atr, resident_flag = unpack("=4xIB", data[offset:offset+9])
            if resident_flag == 0: 
                len_name, offset_name, flags, id_atr, size_data, offset_data, flags_atr = unpack("=9xBHHHIHB", data[offset:offset+23])
                print(f""" 
                Резидентный атрибут
                Идентификатор типа атрибута: {atr[0]}
                Длина атрибута: {size_atr}
                Длина имени: {len_name}
                Смещение имени: {offset_name}
                Флаги: {flags}
                Идентификатор атрибута: {id_atr}
                Длина содержимого: {size_data}
                Смещение содержимого: {offset_data}
                Флаги резидентного атрибута: {flags_atr}
                """)
            elif resident_flag == 1:
                len_name, offset_name, flags, id_atr, index_first_virtual_clust, index_last_virtual_clust, offset_series, size_compres_block, allocated_size_data, real_size_data, initial_size_data  = unpack("=9xBHHHQQHH4xQQQ", data[offset:offset+64])
                if flags == 1:
                    size_atr_after_compres = unpack("=Q", data[offset+64:offset+72])
                else:
                    size_atr_after_compres = (-1,)
                print(f""" 
                Нерезидентный атрибут
                Идентификатор типа атрибута: {atr[0]}
                Длина атрибута: {size_atr}
                Длина имени: {len_name}
                Смещение имени: {offset_name}
                Флаги: {flags}
                Идентификатор атрибута: {id_atr}
                Номер начального виртуального кластера: {index_first_virtual_clust}
                Конечный номер виртуального кластера: {index_last_virtual_clust}
                Cмещение списка серий: {offset_series}
                Размер блока сжатия: {size_compres_block}
                Выделенный размер содержимого атрибута: {allocated_size_data}
                Фактический размер атрибута: {real_size_data}
                Инициализированный размер атрибута: {initial_size_data}
                Размер атрибута после сжатия(если -1 -> не существует): {size_atr_after_compres[0]} 
                """)
            offset = offset + size_atr
            atr = unpack("=I", data[offset:offset+4])
