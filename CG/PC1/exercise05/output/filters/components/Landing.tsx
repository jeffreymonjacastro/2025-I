import Header from "@/components/Header";
import "@/styles/landing.css"

export default function Landing() {
  return (
    <article className="w-full h-[100vh] landing-background">
      <Header />

      <div className="flex flex-col items-center justify-center h-full text-white">
        <h1 className="text-5xl font-bold mb-4">Applying filters to Lenna</h1>
        <p className="text-xl mb-8">CS4016 - Computer Graphics - PC1</p>
      </div>
    </article>
  )
}