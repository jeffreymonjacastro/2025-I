import Filter from "@/components/Filter";
import Footer from "@/components/Footer";
import Landing from "@/components/Landing";
import Image from "next/image";
import { Card, CardContent } from "@/components/ui/card";

export default function Home() {
  return (
    <>
      <Landing />
      
      <main className="w-full text-white ">
        <section className="w-2/3 mx-auto flex flex-col items-center">
          <h2 className="text-3xl font-bold my-2">Original image</h2>
          <p className="mb-3">I will apply different filters to Lenna&apos;s famous picture to see the differences between them. The original one is this:</p>
          <Image
            src="./lenna/lenna.png"
            alt="Original"
            width={500}
            height={500}
            className="rounded-lg shadow-lg"
          />
        </section>

        <section id="box" className="w-full mx-auto flex flex-col items-center">
          <h2 className="text-3xl font-bold mt-4 mb-2">Box filter:</h2>
          <Filter filter_type="box" />
        </section>

        <section id="bartlett" className="w-full mx-auto flex flex-col items-center">
          <h2 className="text-3xl font-bold mt-4 mb-2">Bartlett filter:</h2>
          <Filter filter_type="bartlett" />
        </section>

        <section id="gaussian" className="w-full mx-auto flex flex-col items-center">
          <h2 className="text-3xl font-bold mt-4 mb-2">Gaussian filter:</h2>
          <Filter filter_type="gaussian" />
        </section>
        
        <section id="laplacian" className="w-full mx-auto flex flex-col items-center">
          <h2 className="text-3xl font-bold mt-4 mb-2">Laplacian filter:</h2>
          <div className="flex flex-col md:flex-row justify-around w-full max-w-[90%] md:max-w-[70%]">
            <Card className="p-0 md:mr-4 mb-4 md:mb-0">
              <CardContent className="flex flex-col aspect-squre items-center justify-center p-0">
                <p className="my-1">3 x 3</p>
                <Image
                  src={`./lenna/laplacian_3.png`}
                  alt={`laplacian 3`}
                  width={500}
                  height={500}
                  objectFit="cover"
                  className="rounded-lg shadow-lg"
                />
              </CardContent>
            </Card>
            <Card className="p-0">
              <CardContent className="flex flex-col aspect-squre items-center justify-center p-0">
                <p className="my-1">5 x 5</p>
                <Image
                  src={`./lenna/laplacian_5.png`}
                  alt={`laplacian 5`}
                  width={500}
                  height={500}
                  objectFit="cover"
                  className="rounded-lg shadow-lg"
                />
              </CardContent>
            </Card>
          </div>
        </section>
      </main>

      <Footer />
    </>
  );
}
