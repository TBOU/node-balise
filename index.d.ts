
/*
 Types declarations
 */

declare module "balise" {

    export type BaliseValue = boolean | number | string;

    export function setGlobalOption(name: string, value: string): void;

    export class BaliseProcess {

        constructor();

        loadSourceCode(source: string): void;
        loadSourceFile(path: string): void;
        loadBinaryFile(path: string): void;

        setGlobalVariable(name: string, value: BaliseValue): void;
        getGlobalVariable(name: string): BaliseValue;

        executeFunction(name: string, ...parameters: BaliseValue[]): BaliseValue;
    }
}
